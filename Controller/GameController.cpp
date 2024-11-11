#include "GameController.h"

#include <iostream>
#include "../DataBaseHandler.h"
#include "../dao/UsuarioDAO.h"
#include "../dao/SecuazDAO.h"
#include "../dao/PistaDAO.h"
#include "../dao/LocalidadDAO.h"
#include <memory>
#include <random>
#include <gtkmm.h>

// Constructor de GameController
GameController::GameController(std::shared_ptr<UsuarioDAO> usuarioDAO,
                               std::shared_ptr<SecuazDAO> secuazDAO,
                               std::shared_ptr<PistaDAO> pistaDAO,
                               std::shared_ptr<LocalidadDAO> localidadDAO)
    : usuarioDAO(usuarioDAO), secuazDAO(secuazDAO),
      pistaDAO(pistaDAO), localidadDAO(localidadDAO),
      jugador(),               // Inicializa el usuario
      localidad_actual(),      // Inicializa la localidad actual
      detective(3),           // Inicializa el detective con 3 movimientos
      logueado(false) {       // Inicializa el estado de logueo
}

void GameController::setUsuario(const Usuario& usuario) {
    // Asigna directamente el usuario autenticado
    jugador = usuario;
    detective = Detective(usuario.getRango());
    std::cout << "Usuario asignado: " << jugador.getNombre() << " con rango " << jugador.getRango().getNombre() << std::endl;
}


Localidad GameController::obtenerLocalidadActual() const {
    return localidad_actual;
}

Usuario GameController::obtenerUsuario() const {
    return jugador;  // Devuelve el usuario actual almacenado en GameController
}

//Para pruebas
void GameController::setLocalidadActual(const Localidad& localidad) {
    localidad_actual = localidad;
}

Localidad GameController::obtenerLocalidadAleatoria() {
    std::vector<Localidad> localidades = localidadDAO->getTodasLasLocalidades();

    if (localidades.empty()) {
        std::cerr << "No hay localidades disponibles." << std::endl;
        return Localidad();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, localidades.size() - 1);

    return localidades[distrib(gen)];
}

Secuaz GameController::obtenerSecuazAleatorio() {
    auto secuaces = secuazDAO->getTodosLosSecuaces();
    std::vector<Secuaz> secuaces_disponibles;

    // Filtra secuaces que no han sido capturados y no son Carmen San Diego
    for (const auto& secuaz : secuaces) {
        if (secuaces_capturados.find(secuaz.getId()) == secuaces_capturados.end() &&
            secuaz.getNombre() != "Carmen San Diego") {
            secuaces_disponibles.push_back(secuaz);
            }
    }

    if (secuaces_disponibles.empty()) {
        // Si solo Carmen queda disponible
        Secuaz carmen = secuazDAO->obtenerSecuazPorNombre("Carmen San Diego");
        if (secuaces_capturados.find(carmen.getId()) == secuaces_capturados.end()) {
            return carmen;
        }
    }

    if (!secuaces_disponibles.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, secuaces_disponibles.size() - 1);
        return secuaces_disponibles[distrib(gen)];
    }

    std::cerr << "No hay secuaces disponibles para capturar." << std::endl;
    return Secuaz();
}


void GameController::finalizarJuego(GameWindow* gameWindow) {
    Gtk::MessageDialog* messageDialog = new Gtk::MessageDialog("Te has quedado sin intentos de viajes, casi los tenías!",
                                                               false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE, true);
    messageDialog->add_button("Salir", Gtk::ResponseType::CLOSE);
    messageDialog->set_title("Juego finalizado.");
    messageDialog->show();

    messageDialog->signal_response().connect(
       [messageDialog, gameWindow](int response_id) {
           if (response_id == Gtk::ResponseType::CLOSE) {
               gameWindow->close();
           }
           messageDialog->hide();
           delete messageDialog;
       }
   );
}

void GameController::marcarSecuazComoCapturado(int secuaz_id) {
    secuaces_capturados.insert(secuaz_id);
    usuarioDAO->incrementarCapturas(jugador.getId());
}

bool GameController::haySecuacesDisponibles() const {
    auto secuaces = secuazDAO->getTodosLosSecuaces();
    for (const auto& secuaz : secuaces) {
        if (!secuaz.isCapturado() && secuaz.getNombre() != "Carmen San Diego") {
            return true;
        }
    }
    Secuaz carmen = secuazDAO->obtenerSecuazPorNombre("Carmen San Diego");
    return !carmen.isCapturado();
}

void GameController::actualizarRango() {
    if (jugador.getRango().getNombre() == "Senior") {
        std::cout << "El rango ya es Senior. No se realizarán más actualizaciones de rango." << std::endl;
        return;
    }

    jugador.avanzarRango();
    detective.avanzarRango();  // Avanza el rango del detective también para mantenerlos sincronizados

    if (!usuarioDAO->actualizarRangoUsuario(jugador.getId(), jugador.getRango().getNombre())) {
        std::cerr << "Error al actualizar el rango del usuario en la base de datos." << std::endl;
    } else {
        std::cout << "Rango actualizado en la base de datos a: " << jugador.getRango().getNombre() << std::endl;
    }
}

Rango GameController::obtenerRangoDetective() const {
    return detective.getRango();
}

bool GameController::todosLosSecuacesCapturados() const {
    auto secuaces = secuazDAO->getTodosLosSecuaces();
    for (const auto& secuaz : secuaces) {
        if (secuaz.getNombre() != "Carmen San Diego" && secuaces_capturados.find(secuaz.getId()) == secuaces_capturados.end()) {
            return false;
        }
    }
    return true;
}

Secuaz GameController::obtenerSecuazActual() {
    if (secuaz_actual.getId() == 0) {
        secuaz_actual = obtenerSecuazAleatorio();
    }
    return secuaz_actual;
}

void GameController::capturarSecuazActual(GameWindow *gameWindow) {
    // Marca al secuaz actual como capturado
    secuaces_capturados.insert(secuaz_actual.getId());
    usuarioDAO->incrementarCapturas(jugador.getId());

    // Verifica si el secuaz capturado es Carmen Sandiego
    if (secuaz_actual.getNombre() == "Carmen San Diego") {
        // Llama al método ganarJuego para mostrar la ventana de victoria y finalizar el juego
        ganarJuego(gameWindow);
    } else {
        // Asigna un nuevo secuaz aleatorio si no es Carmen
        secuaz_actual = obtenerSecuazAleatorio();
    }
}


std::pair<Secuaz, Localidad> GameController::iniciarNuevoSecuaz() {
    // Verifica si todos los secuaces disponibles excepto Carmen han sido capturados
    if (todosLosSecuacesCapturados()) {
        Secuaz carmen = secuazDAO->obtenerSecuazPorNombre("Carmen San Diego");
        if (!carmen.isCapturado()) {
            secuaz_actual = carmen;
        } else {
            std::cerr << "Error: Carmen Sandiego ya ha sido capturada." << std::endl;
        }
    } else {
        // Si aún quedan secuaces sin capturar, obtiene uno aleatorio
        secuaz_actual = obtenerSecuazAleatorio();
    }

    // Asigna una nueva localidad aleatoria para el secuaz actual
    localidad_objetivo = obtenerLocalidadAleatoria();
    return {secuaz_actual, localidad_objetivo};
}

//metodo para gestioanr victoria
void GameController::ganarJuego(GameWindow *gameWindow) {
    // Crear un diálogo para mostrar la medalla
    auto* dialog = new Gtk::MessageDialog(*gameWindow, "¡Felicidades!", true, Gtk::MessageType::OTHER, Gtk::ButtonsType::CLOSE, true);
    dialog->set_title("Juego Ganado");

    // Crear la imagen de la medalla final y añadirla al diálogo
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file("../Multimedia/Medallafinal.png");
    Gtk::Image* image_medalla = Gtk::manage(new Gtk::Image(pixbuf->scale_simple(500, 500, Gdk::InterpType::HYPER)));

    dialog->get_content_area()->append(*image_medalla);
    dialog->set_secondary_text("Has atrapado a Carmen Sandiego. ¡Eres un detective legendario!");

    // Conectar la señal para cerrar el juego cuando el usuario cierre el diálogo
    dialog->signal_response().connect([dialog, gameWindow](int) {
        dialog->hide();
        delete dialog;
        gameWindow->close();
    });

    // Mostrar el diálogo con la imagen de la medalla
    dialog->show();
}
