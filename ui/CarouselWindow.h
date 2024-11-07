#ifndef CAROUSEL_WINDOW_H
#define CAROUSEL_WINDOW_H

#include "../Controller/GameController.h"
#include <gtkmm.h>
#include <vector>
#include <string>
#include <filesystem> // Para manejar el sistema de archivos
#include "RegistrationWindow.h"

namespace fs = std::filesystem; // Alias para el namespace

class CarouselWindow : public Gtk::Window {
public:
    CarouselWindow(std::shared_ptr<GameController> gameController, std::shared_ptr<DatabaseHandler> dbHandler);

private:
    Gtk::Box vbox{Gtk::Orientation::VERTICAL}; // Contenedor vertical
    Gtk::Image image_widget; // Widget de imagen
    std::vector<std::string> image_files; // Vector de archivos de imagen
    Glib::RefPtr<Gdk::Pixbuf> pixbuf; // Pixbuf para cargar imágenes
    int current_image_index; // Índice de imagen actual
    std::shared_ptr<GameController> gameController; // Manejador del juego
    std::shared_ptr<RegistrationWindow> registration_window; // Ventana de registro
    std::shared_ptr<DatabaseHandler> dbHandler;

    //Metodos para imagenes y entradas.
    void load_images_from_directory(const std::string& directory_path);
    void load_image();
    bool update_image();
    bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_button_clicked();
};

#endif // CAROUSEL_WINDOW_H
