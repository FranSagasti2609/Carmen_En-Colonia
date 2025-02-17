#include <gtkmm/application.h>
#include <memory>
#include <iostream>
#include <vector>
#include "ui/GameWindow.h"
#include "Controller/GameController.h"
#include "Controller/Globals.h"
#include "Models/Usuario.h"
#include "Models/Localidad.h"
#include "DataBaseHandler.h"
#include "dao/UsuarioDAO.h"
#include "ui/CarouselWindow.h"
#include <iostream>
#include "Models/Usuario.h"
#include "DAO/SecuazDAO.h"
#include "DAO/PistaDAO.h"
#include "DAO/LocalidadDAO.h"
#include "ui/GameWindow.h"


int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("com.example.carmen-sandiego");
    auto mainApp = Gtk::Application::create("com.example.carmen-sandiego");
    std::string db_path = "../carmen_sandiego.db";
    auto dbHandler = std::make_shared<DatabaseHandler>(db_path);

    auto usuarioDAO = std::make_shared<UsuarioDAO>(*dbHandler);
    auto secuazDAO = std::make_shared<SecuazDAO>(*dbHandler);
    auto pistaDAO = std::make_shared<PistaDAO>(*dbHandler);
    auto localidadDAO = std::make_shared<LocalidadDAO>(*dbHandler);

    auto gameController = std::make_shared<GameController>(usuarioDAO, secuazDAO, pistaDAO, localidadDAO);

    // Creo el carrusel de ventanas.
    app->make_window_and_run<CarouselWindow>(argc, argv, gameController, dbHandler);

    // Chequear el estado de logueo después de cerrar CarouselWindow
    if (is_logged_in) {
        std::cout << "Login successful" << std::endl;
        return mainApp->make_window_and_run<GameWindow>(argc, argv, gameController, dbHandler);
    }

    return 0;
}