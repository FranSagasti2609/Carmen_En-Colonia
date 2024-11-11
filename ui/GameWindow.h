#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <gtkmm.h>
#include <memory>
#include "../Controller/GameController.h"
#include "../Models/Usuario.h"
#include "../DAO/PistaDAO.h"
#include "../DataBaseHandler.h"

class GameController;
class GameWindow : public Gtk::Window {
public:
    // Cambiar el tipo de `database_handler` a `std::shared_ptr<DatabaseHandler>`
    explicit GameWindow(const std::shared_ptr<GameController>& game_controller, std::shared_ptr<DatabaseHandler> database_handler);
    void mostrarIntroduccion();
    bool efectoTipeo();  // Callback para añadir caracteres progresivamente
    void iniciarJuego();
    void iniciarNuevoJuego();

    void actualizarLabelRango(const Usuario& usuario_actualizado);
    void mostrarDialogoRango(const std::string &nuevoRango);
    void mostrarDialogoTodosCapturados();
    void seleccionarPista(int indice);
    bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_entry_activate();
    std::vector<Pista> pistas_seleccionadas; // Almacenar las pistas seleccionadas

    //metodos para iconos.
    void actualizarSecuazLabel(const Secuaz& secuaz);
    void actualizarRangoLabel(const Usuario& usuario);

    std::pair<Secuaz, Localidad> iniciarNuevoSecuaz();
    void actualizarRangoYLabel(const Usuario& usuario);
private:
    // Etiquetas y contenedores
    Gtk::Label* label_rango;
    Gtk::Label label_localidad;
    Gtk::Box vbox{Gtk::Orientation::VERTICAL};
    Gtk::Box vbox_pistas{Gtk::Orientation::VERTICAL};
    Gtk::Entry entry_input;
    Gtk::Image imagen_rango;
    Gtk::Label label_introduccion;
    Gtk::Label label_intentos;
    Gtk::Label* label_secuaz; //iconos
    Gtk::Image* image_secuaz; //iconos
    Gtk::Image* image_rango;//iconos

    // Controlador del juego
    std::shared_ptr<GameController> controller;

    // Manejador de pistas
    PistaDAO pistaDAO;

    // Información de localidades
    Localidad localidad_actual;   // Ciudad en la que está el detective
    Localidad localidad_objetivo; // Ciudad donde fue visto el secuaz

    // Funciones auxiliares
    void mostrarPistas(const std::vector<Pista>& pistas);
    void limpiarContenedor(Gtk::Box& contenedor);
    void actualizarIntentos();

    // Variables para efecto de tipeo
    std::string mensaje_completo;  // Mensaje de introducción completo
    std::string mensaje_actual;    // Mensaje que se mostrará progresivamente
    size_t indice_tipeo = 0;       // Índice del caracter actual a mostrar

    // Contador de intentos
    int intentos_actuales = 3;
    int contador_correctas = 0;

    // Para gestionar el ID de secuaces a capturar
    int secuaz_id;
    int contador_racha = 0;  // Contador de pistas correctas consecutivas
    bool mostradoMensajeSenior = false;
};

#endif // GAME_WINDOW_HPP
