#include "RegistrationWindow.h"
#include "../DAO/UsuarioDAO.h"
#include "../Models/Usuario.h"
#include <iostream>

#include "../Controller/Globals.h"

// Constructor
RegistrationWindow::RegistrationWindow(std::shared_ptr<GameController> game_controller,
                                       std::shared_ptr<DatabaseHandler> db_handler,
                                       std::function<void()> closeParentWindow)
    : game_controller(game_controller), dbHandler(db_handler), closeParentWindow(closeParentWindow) {

    set_title("Registro o Login");
    set_default_size(400, 200);

    // Configuración de entradas y botones
    entry_username.set_placeholder_text("Nombre de usuario");
    entry_apellido.set_placeholder_text("Apellido del usuario");
    entry_password.set_placeholder_text("Contraseña");
    entry_password.set_visibility(false);

    // Conectar señales de los botones para el registro y login
    button_register.signal_clicked().connect(sigc::mem_fun(*this, &RegistrationWindow::on_button_register_clicked));
    button_login.signal_clicked().connect(sigc::mem_fun(*this, &RegistrationWindow::login_success));

    // Añadir widgets al contenedor
    vbox.append(entry_username);
    vbox.append(entry_apellido);
    vbox.append(entry_password);
    vbox.append(button_register);
    vbox.append(button_login);
    vbox.append(label_status);

    set_child(vbox);
}

void RegistrationWindow::on_button_register_clicked() {
    std::string nombre = entry_username.get_text();
    std::string apellido = entry_apellido.get_text();
    std::string contrasena = entry_password.get_text();

    UsuarioDAO usuario_dao(*dbHandler);
    if (usuario_dao.registrarUsuario(nombre, apellido, contrasena)) {
        actualizar_estado("Registro exitoso. Ahora inicia sesión.");
    } else {
        actualizar_estado("Usuario ya registrado, logueate.");
    }
}

void RegistrationWindow::login_success() {
    std::string nombre = entry_username.get_text();
    std::string apellido = entry_apellido.get_text();
    std::string contrasena = entry_password.get_text();

    UsuarioDAO usuario_dao(*dbHandler);
    auto usuario = usuario_dao.autenticarUsuario(nombre, apellido, contrasena);

    if (usuario) {
        game_controller->setUsuario(usuario.value());
        std::cout << "El juego comenzará, detective " << usuario->getNombre() << std::endl;

        closeParentWindow(); // Llama al callback que cierra CarouselWindow
        is_logged_in = true; //habilito variable para gestion del main
    } else {
        actualizar_estado("Credenciales incorrectas");
    }
}

void RegistrationWindow::actualizar_estado(const std::string& mensaje) {
    label_status.set_text(mensaje);
}
