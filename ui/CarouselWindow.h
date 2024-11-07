#ifndef CAROUSEL_WINDOW_H
#define CAROUSEL_WINDOW_H

#include <gtkmm.h>
#include <vector>
#include <string>
#include <filesystem>
#include "../Controller/GameController.h"
#include "../DatabaseHandler.h"
#include "RegistrationWindow.h"

namespace fs = std::filesystem;

class CarouselWindow : public Gtk::Window {
public:
    CarouselWindow(std::shared_ptr<GameController> gameController, std::shared_ptr<DatabaseHandler> dbHandler);
    virtual ~CarouselWindow() = default;

    void closeWindow(); // Método para cerrar la ventana

private:
    Gtk::Box vbox{Gtk::Orientation::VERTICAL};
    Gtk::Image image_widget;
    std::vector<std::string> image_files; // Lista de archivos de imagen
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    int current_image_index = 0; // Índice actual de la imagen en el carrusel
    std::shared_ptr<GameController> gameController;
    std::shared_ptr<DatabaseHandler> dbHandler;
    std::shared_ptr<RegistrationWindow> registration_window;

    void load_images_from_directory(const std::string& directory_path);
    void load_image();
    bool update_image();
    bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void createRegistrationWindow(); // Método para crear la ventana de registro con un callback
};

#endif // CAROUSEL_WINDOW_H
