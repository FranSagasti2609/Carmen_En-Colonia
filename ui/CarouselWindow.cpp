    #include "CarouselWindow.h"
    #include <iostream>
    #include <filesystem>
    #include <gdk/gdkkeysyms.h> // Para constantes de teclas
    #include "../ui/RegistrationWindow.h"
    #include "../Controller/Globals.h"
    namespace fs = std::filesystem;

CarouselWindow::CarouselWindow(std::shared_ptr<GameController> gameController, std::shared_ptr<DatabaseHandler> dbHandler)
    : gameController(std::move(gameController)), dbHandler(std::move(dbHandler)), registration_window(nullptr) {
        set_title("Carmen Sandiego en Colonia!");
        set_default_size(800, 600);

        // Configurar la ventana para capturar eventos de teclado
        auto keyController = Gtk::EventControllerKey::create();
        keyController->signal_key_pressed().connect(sigc::mem_fun(*this, &CarouselWindow::on_key_pressed), false);
        add_controller(keyController);

        vbox.set_orientation(Gtk::Orientation::VERTICAL);
        set_child(vbox);
        set_focus(vbox); // Aseguramos que el foco está en el contenedor principal

        // Cargar imágenes desde el directorio
        load_images_from_directory("../Localidades_Photos/");
        if (image_files.empty()) {
            std::cerr << "No se encontraron imágenes en el directorio." << std::endl;
            return;
        }

        // Inicializar widget de imagen y agregarlo al contenedor
        vbox.append(image_widget);
        image_widget.set_hexpand(true);
        image_widget.set_vexpand(true);
        load_image();

        // Crear y configurar el botón (opcional: también se podría iniciar el juego con el botón)
        auto buttonInicioGame = Gtk::Button("Pulsa BORRAR o BACKSPACE para iniciar la aventura.");
;        vbox.append(buttonInicioGame);

        // Mostrar todos los widgets
        show();

        // Iniciar el carrusel
        Glib::signal_timeout().connect(sigc::mem_fun(*this, &CarouselWindow::update_image), 2000);

    }

    void CarouselWindow::load_images_from_directory(const std::string& directory_path) {
        for (const auto& entry : fs::directory_iterator(directory_path)) {
            if (entry.is_regular_file()) {
                std::string extension = entry.path().extension().string();
                if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                    image_files.push_back(entry.path().string());
                }
            }
        }
    }

    void CarouselWindow::load_image() {
        if (image_files.empty()) {
            std::cerr << "Error: no hay imágenes cargadas para mostrar." << std::endl;
            return;
        }

        pixbuf = Gdk::Pixbuf::create_from_file(image_files[current_image_index]);
        pixbuf = pixbuf->scale_simple(1280, 720, Gdk::InterpType::BILINEAR);
        image_widget.set(pixbuf);
    }

    bool CarouselWindow::update_image() {
        if (image_files.empty()) return false;

        current_image_index = (current_image_index + 1) % image_files.size();
        load_image();
        return true;
    }

bool CarouselWindow::on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state) {
    if (keyval == GDK_KEY_BackSpace) { // o usa 65288 como alternativa
        if (!is_logged_in) {
            if (!registration_window) {
                registration_window = std::make_shared<RegistrationWindow>(gameController, dbHandler);
                registration_window->signal_hide().connect([this]() {
                    registration_window.reset(); // Liberar el registro al cerrarse
                });
            }
            registration_window->show();
        } else {
            std::cout << "Ya has iniciado sesión." << std::endl;
        }
        return true;
    }
    return false;
}

