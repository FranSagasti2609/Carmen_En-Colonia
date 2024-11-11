#include "GameWindow.h"
#include <sstream>
#include <iostream>
#include <random>
#include <gtkmm/messagedialog.h>

#include "../Controller/Globals.h"
#include "../Models/Rango.h"
#include "../Models/Secuaz.h"

// Constructor de GameWindow
GameWindow::GameWindow(const std::shared_ptr<GameController>& game_controller, std::shared_ptr<DatabaseHandler> db_handler)
    : controller(game_controller), pistaDAO(*db_handler) {

    set_title("Carmen Sandiego en Colonia - Juego");
    set_default_size(1160, 600);

    Usuario usuario = controller->obtenerUsuario();
    Rango rango = usuario.getRango();
    Secuaz secuaz = controller->obtenerSecuazActual();

    Gtk::Box* hbox_main = Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 10));

    // Sección Izquierda: Información del detective y rango
    Gtk::Box* vbox_left = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5));
    image_rango = Gtk::manage(new Gtk::Image(rango.getImagePath()));
    image_rango->set_size_request(150, 150);
    label_rango = Gtk::manage(new Gtk::Label("Rango: " + rango.getNombre()));
    Gtk::Label* label_nombre = Gtk::manage(new Gtk::Label("Detective: " + usuario.getNombre()));
    label_intentos.set_text("Intentos actuales: " + std::to_string(intentos_actuales));
    vbox_left->append(*image_rango);
    vbox_left->append(*label_rango);
    vbox_left->append(*label_nombre);
    vbox_left->append(label_intentos);
    hbox_main->append(*vbox_left);

    // Sección Central: Pistas y Entrada de Texto
    Gtk::Box* vbox_center = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5));
    label_introduccion.set_margin_top(5);
    label_introduccion.set_halign(Gtk::Align::CENTER);
    vbox_center->append(label_introduccion);
    entry_input.set_placeholder_text("Ingresa D, F o G para seleccionar pista");
    entry_input.signal_activate().connect(sigc::mem_fun(*this, &GameWindow::on_entry_activate));
    vbox_center->append(entry_input);
    vbox_center->append(vbox_pistas);
    hbox_main->append(*vbox_center);

    // Sección Derecha: Información del secuaz
    Gtk::Box* vbox_right = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 5));
    image_secuaz = Gtk::manage(new Gtk::Image(secuaz.getImagePath()));
    image_secuaz->set_size_request(150, 150);
    label_secuaz = Gtk::manage(new Gtk::Label("Secuaz: " + secuaz.getNombre()));
    vbox_right->append(*image_secuaz);
    vbox_right->append(*label_secuaz);
    hbox_main->append(*vbox_right);

    set_child(*hbox_main);
    show();

    mostrarIntroduccion();
    iniciarJuego();

}

void GameWindow::actualizarLabelRango(const Usuario& usuario) {

    if (usuario.getRango().getNombre() == "Senior") {
        // Si el rango ya es Senior, no se actualiza el label para evitar cambios
        return;
    }
    std::string rango = usuario.getRango().getNombre();
    std::string rangoText = "Detective: " + usuario.getNombre() + " | Rango: " + rango;
    label_rango->set_text(rangoText);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(usuario.getRango().getImagePath());
    image_rango->set(pixbuf->scale_simple(100, 100, Gdk::InterpType::BILINEAR));
}

void GameWindow::mostrarIntroduccion() {
    // Obtiene el secuaz actual. Si no hay secuaz asignado (ID == 0), selecciona uno nuevo.
    Secuaz secuaz = controller->obtenerSecuazActual();
    if (secuaz.getId() == 0) {
        auto [nuevoSecuaz, nuevaLocalidad] = controller->iniciarNuevoSecuaz();
        secuaz = nuevoSecuaz;
        localidad_objetivo = nuevaLocalidad;
    } else {
        // Si ya hay un secuaz asignado, solo actualiza la localidad objetivo
        localidad_objetivo = controller->obtenerLocalidadAleatoria();
    }

    secuaz_id = secuaz.getId();

    // Genera una localidad aleatoria para la ubicación actual del detective, distinta de la localidad objetivo
    do {
        localidad_actual = controller->obtenerLocalidadAleatoria();
    } while (localidad_actual.getNombre() == localidad_objetivo.getNombre());

    // Reinicia el mensaje de introducción y configura la localidad actual en el controlador
    mensaje_actual.clear();
    label_introduccion.set_text("");
    controller->setLocalidadActual(localidad_actual);

    // Construye el mensaje de introducción con los detalles del secuaz y las localidades
    std::ostringstream oss;
    if (secuaz.getNombre() == "Carmen San Diego") {
        oss << "Mensaje Urgente de ACME: Has rastreado a la villana Carmen Sandiego, ha sido vista en "
            << localidad_objetivo.getNombre()
            << ". Tu misión es capturarla para derrocar esta liga de villanos.\n"
            << "Actualmente estás en " << localidad_actual.getNombre() << ". ¡Buena suerte, detective!";
    } else {
        oss << "Mensaje Urgente de ACME: El secuaz " << secuaz.getNombre()
            << " ha sido visto en " << localidad_objetivo.getNombre()
            << ". Tu misión es capturarlo para interrogarlo lo antes posible.\n"
            << "Actualmente estás en " << localidad_actual.getNombre() << ". ¡Buena suerte, detective!";
    }

    // Configura el mensaje completo y el efecto de tipeo
    mensaje_completo = oss.str();
    mensaje_actual.clear();
    indice_tipeo = 0;
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &GameWindow::efectoTipeo), 50);

    // Actualiza la interfaz con la imagen y el nombre del secuaz
    actualizarSecuazLabel(secuaz);
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
        Gtk::MessageDialog* dialog;

        // Verifica si la pista seleccionada es correcta
        if (pistas_seleccionadas[indice].getEsCorrecta()) {
            contador_racha++;  // Incrementa el contador de aciertos

            // Muestra mensaje de pista correcta
            dialog = new Gtk::MessageDialog(*this, "¡Pista Correcta!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
            dialog->signal_response().connect([this, dialog](int) {
                dialog->hide();
                delete dialog;

                // Verifica si se alcanzaron 3 aciertos
                if (contador_racha == 1) {
                    controller->capturarSecuazActual();  // Captura el secuaz actual
                    contador_racha = 0;  // Reinicia la racha

                    // Verifica si todos los secuaces han sido capturados
                    if (controller->todosLosSecuacesCapturados()) {
                        mostrarDialogoTodosCapturados();

                        // Asigna a Carmen Sandiego como el secuaz final si el jugador tiene el rango necesario
                        if (controller->obtenerUsuario().getRango().getNombre() == "Senior") {
                            auto [nuevoSecuaz, nuevaLocalidad] = controller->iniciarNuevoSecuaz();
                            actualizarSecuazLabel(nuevoSecuaz);  // Actualiza la interfaz con Carmen Sandiego
                            localidad_objetivo = nuevaLocalidad;
                            mostrarIntroduccion();

                            // Finaliza el juego después de capturar a Carmen Sandiego
                            if (nuevoSecuaz.getNombre() == "Carmen San Diego") {
                                controller->finalizarJuego(this);
                                return;
                            }

                        } else {
                            // Si no tiene el rango "Senior", muestra un mensaje y finaliza el juego
                            Gtk::MessageDialog* dialogFaltaRango = new Gtk::MessageDialog(
                                *this, "¡Todos los secuaces han sido capturados!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
                            dialogFaltaRango->set_secondary_text("Detective, necesitas alcanzar el rango 'Senior' para intentar capturar a Carmen Sandiego.");
                            dialogFaltaRango->signal_response().connect([dialogFaltaRango, this](int) {
                                dialogFaltaRango->hide();
                                delete dialogFaltaRango;
                                controller->finalizarJuego(this);  // Finaliza el juego después de cerrar el diálogo
                            });
                            dialogFaltaRango->show();
                            return;
                        }
                    } else {
                        // Selecciona un nuevo secuaz y una nueva localidad
                        auto [nuevoSecuaz, nuevaLocalidad] = controller->iniciarNuevoSecuaz();
                        actualizarSecuazLabel(nuevoSecuaz);  // Actualiza la interfaz con el nuevo secuaz
                        localidad_objetivo = nuevaLocalidad;
                        mostrarIntroduccion();
                    }

                    // Actualiza el rango del detective si aún no es "Senior"
                    if (controller->obtenerRangoDetective().getNombre() != "Senior") {
                        controller->actualizarRango();
                        actualizarRangoYLabel(controller->obtenerUsuario());  // Actualiza rango y etiqueta solo si no es "Senior"

                            // Verifica nuevamente si el rango sigue siendo distinto de "Senior" después de la actualización
                        if (controller->obtenerRangoDetective().getNombre() != "Senior") {
                               mostrarDialogoRango(controller->obtenerRangoDetective().getNombre());
                         }
                        }
                } else {
                    // Si no se alcanzaron 3 aciertos, actualiza solo la localidad sin cambiar el secuaz
                    localidad_objetivo = controller->obtenerLocalidadAleatoria();
                    mostrarIntroduccion();
                }
            });
            dialog->show();
        } else {
            // Si la pista seleccionada es incorrecta, reinicia la racha
            contador_racha = 0;
            dialog = new Gtk::MessageDialog(*this, "Pista Incorrecta. Inténtalo nuevamente.", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK);
            intentos_actuales--;  // Reduce los intentos restantes
            actualizarIntentos();  // Actualiza la interfaz con los intentos restantes
            dialog->signal_response().connect([this, dialog](int) {
                dialog->hide();
                delete dialog;

                // Si los intentos llegan a 0, finaliza el juego
                if (intentos_actuales == 0) {
                    controller->finalizarJuego(this);
                }
            });
            dialog->show();
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
            label_pista->set_use_markup(true);
            label_pista->set_markup(pista_con_letra.str());

            vbox_pistas.append(*label_pista);
        }
    }
    vbox_pistas.show();
}

void GameWindow::limpiarContenedor(Gtk::Box& contenedor) {
    for (auto& widget : contenedor.get_children()) {
        contenedor.remove(*widget);
    }
}

void GameWindow::on_entry_activate() {
    std::string input = entry_input.get_text();
    if (input == "D" || input == "d") {
        seleccionarPista(0);
    } else if (input == "F" || input == "f") {
        seleccionarPista(1);
    } else if (input == "G" || input == "g") {
        seleccionarPista(2);
    }
    entry_input.set_text("");
}

void GameWindow::actualizarIntentos() {
    label_intentos.set_text("Intentos actuales: " + std::to_string(intentos_actuales));
}

void GameWindow::mostrarDialogoRango(const std::string& nuevoRango) {
    if (nuevoRango == "Senior") {
        // Solo muestra el diálogo una vez, no cada vez que el juego lo llama en cada captura
            std::string mensaje = "¡Felicidades! Ahora eres un detective Senior. Puedes intentar capturar a Carmen San Diego.";
            Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, "¡Rango Aumentado!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
            dialog->set_secondary_text(mensaje);
            dialog->signal_response().connect([dialog](int) {
                dialog->hide();
                delete dialog;
            });
            dialog->show();
    } else {
        // Muestra el mensaje estándar para rangos diferentes a "Senior"
        std::string mensaje = "¡Felicidades! Ahora eres " + nuevoRango + ".";
        Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, "¡Rango Aumentado!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
        dialog->set_secondary_text(mensaje);
        dialog->signal_response().connect([dialog](int) {
            dialog->hide();
            delete dialog;
        });
        dialog->show();
    }
}


void GameWindow::mostrarDialogoTodosCapturados() {
    Gtk::MessageDialog* dialog = new Gtk::MessageDialog(*this, "¡Todos los secuaces han sido capturados!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
    dialog->set_secondary_text("Felicidades, detective. Solo queda enfrentar a Carmen Sandiego.");
    dialog->signal_response().connect([dialog](int) {
        dialog->hide();
        delete dialog;
    });
    dialog->show();
}

void GameWindow::actualizarSecuazLabel(const Secuaz& secuaz) {
    label_secuaz->set_text("Secuaz: " + secuaz.getNombre());
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(secuaz.getImagePath());
    image_secuaz->set(pixbuf->scale_simple(150, 150, Gdk::InterpType::BILINEAR));
}

void GameWindow::actualizarRangoYLabel(const Usuario& usuario) {
    std::string rango_actual = label_rango->get_text();  // Texto actual del rango en la interfaz

    if (usuario.getRango().getNombre() == "Senior" && rango_actual.find("Senior") != std::string::npos) {
        return;  // Evita actualizaciones si ya se alcanzó "Senior" previamente
    }

    // Actualiza el texto y la imagen de rango
    std::string rangoTexto = "Rango: " + usuario.getRango().getNombre();
    label_rango->set_text(rangoTexto);

    // Actualiza la imagen del rango
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(usuario.getRango().getImagePath());
    image_rango->set(pixbuf->scale_simple(150, 150, Gdk::InterpType::BILINEAR));

    // Muestra una ventana de diálogo solo si se alcanza "Senior" por primera vez
    if (usuario.getRango().getNombre() == "Senior" && rango_actual.find("Senior") == std::string::npos) {
        Gtk::MessageDialog* dialog = new Gtk::MessageDialog(
            *this, "¡Rango Máximo Alcanzado!", false, Gtk::MessageType::INFO, Gtk::ButtonsType::OK, true);
        dialog->set_secondary_text("¡Felicidades! Ahora eres un detective Senior y puedes intentar capturar a Carmen Sandiego.");
        dialog->signal_response().connect([dialog](int) {
            dialog->hide();
            delete dialog;
        });
        dialog->show();
    }
}


