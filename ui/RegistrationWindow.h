#ifndef REGISTRATION_WINDOW_H
#define REGISTRATION_WINDOW_H

#include <gtkmm/applicationwindow.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <memory>
#include "../Controller/GameController.h"
#include "GameWindow.h"

class RegistrationWindow : public Gtk::ApplicationWindow {
public:
    explicit RegistrationWindow(std::shared_ptr<GameController> game_controller, std::shared_ptr<DatabaseHandler> dbHandler);
    virtual ~RegistrationWindow() = default;

private:
    void on_button_register_clicked();
    void login_success();
    void actualizar_estado(const std::string& mensaje);

    // Variables de miembro
    std::shared_ptr<GameController> game_controller;
    std::shared_ptr<DatabaseHandler> dbHandler;

    Gtk::Entry entry_username;
    Gtk::Entry entry_apellido;
    Gtk::Entry entry_password;
    Gtk::Button button_register{"Registrarse"};
    Gtk::Button button_login{"Iniciar sesión"};
    Gtk::Label label_status;
    Gtk::Box vbox{Gtk::Orientation::VERTICAL};
};

#endif // REGISTRATION_WINDOW_H
