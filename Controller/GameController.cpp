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
    if (detective.getRango().getNombre() == "Senior") {
        std::cout << "El rango ya es Senior. No se realizarán más actualizaciones de rango." << std::endl;
        return;
    }
    detective.avanzarRango();

    if (!usuarioDAO->actualizarRangoUsuario(jugador.getId(), detective.getRango().getNombre())) {
        std::cerr << "Error al actualizar el rango del usuario en la base de datos." << std::endl;
    } else {
        std::cout << "Rango actualizado en la base de datos a: " << detective.getRango().getNombre() << std::endl;
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

void GameController::capturarSecuazActual() {
    secuaces_capturados.insert(secuaz_actual.getId());
    usuarioDAO->incrementarCapturas(jugador.getId());
    secuaz_actual = obtenerSecuazAleatorio();
}

std::pair<Secuaz, Localidad> GameController::iniciarNuevoSecuaz() {
    secuaz_actual = obtenerSecuazAleatorio();
    localidad_objetivo = obtenerLocalidadAleatoria();
    return {secuaz_actual, localidad_objetivo};
}
