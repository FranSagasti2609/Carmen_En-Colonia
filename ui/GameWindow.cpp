#include "GameWindow.h"
#include <sstream>
#include <iostream>
#include <random>
#include <gtkmm/messagedialog.h>
#include "../Models/Rango.h"

// Constructor de GameWindow
// Constructor de GameWindow
GameWindow::GameWindow(const std::shared_ptr<GameController>& game_controller, std::shared_ptr<DatabaseHandler> db_handler)
    : controller(game_controller), pistaDAO(*db_handler) {

    set_title("Carmen Sandiego en Colonia - Juego");
    set_default_size(800, 600);

    // Configuración de la información del detective
    Usuario usuario = controller->obtenerUsuario();

    // Crear un contenedor horizontal para la imagen y el texto
    Gtk::Box* hbox_rango = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 200));

    // Texto del rango
    actualizarLabelRango(controller->obtenerUsuario());
    hbox_rango->append(label_rango);

    // Configuración de intentos
    label_intentos.set_text("Intentos actuales: " + std::to_string(intentos_actuales));
    hbox_rango->append(label_intentos);

    vbox.append(*hbox_rango);

    // Agregar el label de introducción con efecto de tipeo al contenedor
    vbox.append(label_introduccion);

    // Contenedor para las pistas
    vbox_pistas.set_margin_top(10);
    vbox.append(vbox_pistas);

    // Llama a mostrarIntroduccion() para inicializar la presentación
    mostrarIntroduccion();

    // Agregar campo de entrada
    entry_input.set_placeholder_text("Ingresa D, F o G para seleccionar pista");
    entry_input.signal_activate().connect(sigc::mem_fun(*this, &GameWindow::on_entry_activate));
    vbox.append(entry_input);

    set_child(vbox);
}

void GameWindow::mostrarIntroduccion() {
    // Obtener secuaz y localidades aleatorias
    Secuaz secuaz = controller->obtenerSecuazAleatorio();
    secuaz_id = secuaz.getId();
    localidad_objetivo = controller->obtenerLocalidadAleatoria();  // Ciudad objetivo (donde fue visto el secuaz)

    do {
        localidad_actual = controller->obtenerLocalidadAleatoria();  // Ciudad actual del detective
    } while(localidad_actual.getNombre() == localidad_objetivo.getNombre());

    // Reiniciar labels
    mensaje_actual.clear();
    label_introduccion.set_text("");

    // Mostrar ciudad actual y ciudad objetivo
    controller->setLocalidadActual(localidad_actual);

    // Crear el mensaje completo de ACME
    std::ostringstream oss;
    oss << "Mensaje Urgente de ACME: El secuaz " << secuaz.getNombre()
        << " ha sido visto en " << localidad_objetivo.getNombre()
        << ". Tu misión es capturarlo para interrogarlo lo antes posible.\n"
        << "Actualmente estás en " << localidad_actual.getNombre() << ". ¡Buena suerte, detective!";

    // Inicia el efecto de tipeo para mostrar el mensaje
    mensaje_completo = oss.str();
    mensaje_actual.clear();
    indice_tipeo = 0;
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::efectoTipeo), 50);
}

bool GameWindow::efectoTipeo() {
    if (indice_tipeo < mensaje_completo.size()) {
        mensaje_actual += mensaje_completo[indice_tipeo++];
        label_introduccion.set_use_markup(true);
        label_introduccion.set_markup(mensaje_actual);
        return true;
    } else {
        iniciarJuego();
        return false;
    }
}

void GameWindow::iniciarJuego() {
    // Obtener todas las pistas de la ciudad objetivo
    auto pistas = pistaDAO.obtenerPistasPorLocalidad(localidad_objetivo.getId());

    Pista pista_correcta;
    std::vector<Pista> pistas_incorrectas;
    for (const auto& pista : pistas) {
        if (pista.getEsCorrecta()) {
            pista_correcta = pista;
        } else {
            pistas_incorrectas.push_back(pista);
        }
    }

    if (pista_correcta.getDescripcion().empty() || pistas_incorrectas.size() < 2) {
        std::cerr << "Faltan pistas correctas o incorrectas suficientes." << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pistas_incorrectas.begin(), pistas_incorrectas.end(), gen);
    std::vector<Pista> pistas_seleccionadas = {pista_correcta, pistas_incorrectas[0], pistas_incorrectas[1]};

    std::shuffle(pistas_seleccionadas.begin(), pistas_seleccionadas.end(), gen);
    this->pistas_seleccionadas = pistas_seleccionadas;

    mostrarPistas(pistas_seleccionadas);
}

void GameWindow::seleccionarPista(int indice) {
    if (indice >= 0 && indice < pistas_seleccionadas.size()) {
        Gtk::MessageDialog *dialog;

        if (pistas_seleccionadas[indice].getEsCorrecta()) {
            contador_racha++;

            // Mensaje de pista correcta
            dialog = new Gtk::MessageDialog(*this, "¡Pista Correcta!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK);
            dialog->signal_response().connect([this, dialog](int) {
                dialog->hide();
                delete dialog; // Eliminar el diálogo correctamente

                // Si alcanza una racha de 3 aciertos, captura al secuaz
                if (contador_racha == 3) {
                    controller->marcarSecuazComoCapturado(secuaz_id);
                    contador_racha = 0;  // Reinicia la racha después de capturar

                    // Verifica si todos los secuaces han sido capturados
                    if (controller->todosLosSecuacesCapturados()) {
                        mostrarDialogoTodosCapturados();  // Muestra el mensaje de captura final
                    } else {
                        iniciarNuevoJuego();  // Continúa con un nuevo secuaz si quedan disponibles
                    }
                    // Actualiza el rango del detective
                    controller->actualizarRango();
                    Usuario usuario_actualizado = controller->obtenerUsuario();
                    actualizarLabelRango(usuario_actualizado);

                    std::string nuevoRango = controller->obtenerRangoDetective().getNombre();
                    mostrarDialogoRango(nuevoRango);  // Muestra el mensaje de rango actualizado


                } else {
                    // Si no alcanza la racha de 3, el secuaz se mueve de localidad y se generan nuevas pistas
                    localidad_objetivo = controller->obtenerLocalidadAleatoria();  // Actualiza la nueva ubicación del secuaz
                    mostrarIntroduccion();  // Genera nuevas pistas para la nueva ubicación
                }
            });
            dialog->show(); // Mostrar el diálogo de pista correcta

        } else {
            contador_racha = 0; // Pierde la racha si elige una pista incorrecta
            // Mensaje para pista incorrecta
            dialog = new Gtk::MessageDialog(*this,
                                            "Pista Incorrecta. Inténtalo nuevamente.",
                                            false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK);
            intentos_actuales--;
            actualizarIntentos();
            dialog->signal_response().connect([this, dialog](int) {
                dialog->hide();
                delete dialog; // Eliminar el diálogo correctamente

                if (intentos_actuales == 0) {
                    controller->finalizarJuego(this);
                }
            });
            dialog->show(); // Mostrar el diálogo
        }
    } else {
        std::cerr << "Índice de pista no válido." << std::endl;
    }
}

void GameWindow::mostrarPistas(const std::vector<Pista>& pistas) {
    limpiarContenedor(vbox_pistas);
    std::vector<char> letras = {'D', 'F', 'G'};

    for (size_t i = 0; i < pistas.size(); ++i) {
        if (i < letras.size()) {
            std::ostringstream pista_con_letra;
            pista_con_letra << "<span font='11' weight='bold'>" << letras[i] << ": " << pistas[i].getDescripcion() << "</span>";

            Gtk::Label* label_pista = Gtk::manage(new Gtk::Label());
            label_pista->set_use_markup(true);  // Habilita el uso de markup
            label_pista->set_markup(pista_con_letra.str());
            vbox_pistas.append(*label_pista);
        }
    }
    vbox_pistas.show();
}


// Implementación de limpiarContenedor
void GameWindow::limpiarContenedor(Gtk::Box& contenedor) {
    // Remueve y destruye todos los widgets en el contenedor
    for (auto& widget : contenedor.get_children()) {
        contenedor.remove(*widget);
    }
}

// Implementación de on_entry_activate
void GameWindow::on_entry_activate() {
    std::string input = entry_input.get_text();
    if (input == "D" || input == "d") {
        seleccionarPista(0);
    } else if (input == "F" || input == "f") {
        seleccionarPista(1);
    } else if (input == "G" || input == "g") {
        seleccionarPista(2);
    }
    entry_input.set_text("");  // Limpia el campo de entrada después de procesar
}

void GameWindow::actualizarIntentos() {
    label_intentos.set_text("Intentos actuales: " + std::to_string(intentos_actuales));
}

void GameWindow::iniciarNuevoJuego() {
    localidad_objetivo = controller->obtenerLocalidadAleatoria();
    mostrarIntroduccion();  // Muestra la introducción para el nuevo secuaz
}

void GameWindow::actualizarLabelRango(const Usuario& usuario) {
    Glib::signal_idle().connect([this, usuario]() {
        std::ostringstream oss;
        oss << "Detective: " << usuario.getNombre() << " | Rango: " << usuario.getRango().getNombre();
        label_rango.set_text(oss.str());
        return false; // Detener después de la primera ejecución
    });
}


void GameWindow::mostrarDialogoRango(const std::string& nuevoRango) {
    std::string titulo = "¡Rango Aumentado!";
    std::string mensaje;

    if (nuevoRango == "Senior") {
        mensaje = "¡Felicidades! Ahora eres un detective Senior. Puedes intentar capturar a Carmen San Diego.";
        // Aquí puedes también llamar a otras funciones que manejen lógica específica para cuando se alcanza el rango Senior.
    } else {
        mensaje = "¡Felicidades! Ahora eres " + nuevoRango + ".";
    }

    Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, titulo, false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
    dialog->set_secondary_text(mensaje);
    dialog->signal_response().connect([this, dialog](int) {
        dialog->hide();
        delete dialog;
    });
    dialog->show();  // Muestra el diálogo
}


void GameWindow::mostrarDialogoTodosCapturados() {
    Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, "¡Todos los secuaces han sido capturados!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
    dialog->set_secondary_text("Felicidades, detective. Solo queda enfrentar a Carmen Sandiego.");
    dialog->signal_response().connect([dialog](int) {
        dialog->hide();
        delete dialog;
    });
    dialog->show();  // Muestra el diálogo
}
