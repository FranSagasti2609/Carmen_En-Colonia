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
    jugador = usuario;  // Solo asignación
    std::cout << "Usuario asignado: " << usuario.getNombre() << std::endl;
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

    // Verificar que haya localidades disponibles
    if (localidades.empty()) {
        // Manejo del caso donde no hay localidades
        std::cerr << "No hay localidades disponibles." << std::endl;
        return Localidad(); // O retornar una localidad válida por defecto
    }

    // Generar un número aleatorio entre 0 y el tamaño del vector menos uno
    std::random_device rd;  // Obtener una semilla
    std::mt19937 gen(rd()); // Usar Mersenne Twister como generador
    std::uniform_int_distribution<> distrib(0, localidades.size() - 1); // Distribución uniforme

    // Obtener un índice aleatorio
    int indiceAleatorio = distrib(gen);

    // Retornar la localidad aleatoria
    return localidades[indiceAleatorio];
}

Secuaz GameController::obtenerSecuazAleatorio() {
    // Primero, obtén todos los secuaces
    auto secuaces = secuazDAO->getTodosLosSecuaces();

    // Filtra para obtener solo los secuaces que no han sido capturados y no incluyas a Carmen Sandiego
    std::vector<Secuaz> secuaces_disponibles;
    for (const auto& secuaz : secuaces) {
        if (!secuaz.isCapturado() && secuaz.getNombre() != "Carmen San Diego") {
            secuaces_disponibles.push_back(secuaz);
        }
    }

    // Si todos los secuaces (excepto Carmen) han sido capturados, permite capturar a Carmen
    if (secuaces_disponibles.empty()) {
        Secuaz carmen = secuazDAO->obtenerSecuazPorNombre("Carmen San Diego");
        if (!carmen.isCapturado()) {
            return carmen;
        }
    }

    // Selecciona aleatoriamente uno de los secuaces disponibles
    if (!secuaces_disponibles.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, secuaces_disponibles.size() - 1);
        return secuaces_disponibles[distrib(gen)];
    }

    // En caso de que no haya secuaces disponibles
    std::cerr << "No hay secuaces disponibles para capturar." << std::endl;
    return Secuaz(); // Retorna un secuaz vacío o de valor nulo si no hay disponibles
}

void GameController::finalizarJuego(GameWindow* gameWindow) {
    // Crear el diálogo de mensaje de fin de juego
    Gtk::MessageDialog *messageDialog = new Gtk::MessageDialog("Te has quedado sin intentos de viajes, casi los tenías!",
                                                               false, Gtk::MessageType::INFO, Gtk::ButtonsType::NONE, true);

    // Personalizar el diálogo con solo el botón de "Salir"
    messageDialog->add_button("Salir", Gtk::ResponseType::CLOSE);
    messageDialog->set_title("Juego finalizado.");
    messageDialog->show();

    // Conectar la respuesta del usuario al botón "Salir"
    messageDialog->signal_response().connect(
       [messageDialog, gameWindow](int response_id) {
           if (response_id == Gtk::ResponseType::CLOSE) {
               gameWindow->close(); // Cerrar directamente la ventana de juego
           }
           // Destruir el diálogo después de manejar la respuesta
           messageDialog->hide();
           delete messageDialog; // Liberar el recurso
       }
   );
}


void GameController::marcarSecuazComoCapturado(int secuaz_id) {
    secuaces_capturados.insert(secuaz_id);  // Agrega el ID del secuaz capturado
    for (auto secuaz : secuaces_capturados) {
        std::cout << secuaz << " ";
    }
    std::cout << std::endl;
}

bool GameController::haySecuacesDisponibles() const {
    auto secuaces = secuazDAO->getTodosLosSecuaces();
    bool otrosSecuacesDisponibles = false;

    for (const auto& secuaz : secuaces) {
        // Si hay algún secuaz no capturado que no sea Carmen Sandiego, marcamos como disponible
        if (!secuaz.isCapturado() && secuaz.getNombre() != "Carmen San Diego") {
            otrosSecuacesDisponibles = true;
            break;
        }
    }

    // Si no hay otros secuaces disponibles y Carmen no ha sido capturada, entonces está disponible
    if (!otrosSecuacesDisponibles) {
        Secuaz carmen = secuazDAO->obtenerSecuazPorNombre("Carmen San Diego");
        return !carmen.isCapturado(); // True si Carmen está disponible, false si ya fue capturada
    }

    return otrosSecuacesDisponibles;
}

void GameController::actualizarRango() {
    detective.avanzarRango();  // Avanza el rango del detective

    if (!usuarioDAO->actualizarRangoUsuario(jugador.getId(), detective.getRango().getNombre())) {
        std::cerr << "Error al actualizar el rango del usuario en la base de datos." << std::endl;
    } else {
        std::cout << "Rango actualizado en la base de datos a: " << detective.getRango().getNombre() << std::endl;
    }

    if (detective.getRango().getNombre() == "Senior") {
        std::cout << "¡Felicidades! Ahora tienes el rango más alto y puedes capturar a Carmen San Diego." << std::endl;
    }
}

Rango GameController::obtenerRangoDetective() const {
    return detective.getRango();
}
//Metodo para gestionar si los secuaces menos carmen estan capturados
bool GameController::todosLosSecuacesCapturados() const {
    auto secuaces = secuazDAO->getTodosLosSecuaces();
    for (const auto& secuaz : secuaces) {
        if (secuaz.getNombre() != "Carmen San Diego" && secuaces_capturados.find(secuaz.getId()) == secuaces_capturados.end()) {
            return false;
        }
    }
    return true;
}




