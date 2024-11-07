#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include "../DAO/UsuarioDAO.h"
#include "../DAO/SecuazDAO.h"
#include "../DAO/PistaDAO.h"
#include "../DAO/LocalidadDAO.h"
#include "../Models/Usuario.h"
#include "../Models/Detective.h"  // Asegúrate de incluir la cabecera de Detective
#include "../ui/GameWindow.h"
#include <set>

class GameWindow;
enum GameState { INICIO, EN_PROGRESO, FINALIZADO };

class GameController {
public:
    GameController(std::shared_ptr<UsuarioDAO> usuarioDAO, std::shared_ptr<SecuazDAO> secuazDAO,
                   std::shared_ptr<PistaDAO> pistaDAO, std::shared_ptr<LocalidadDAO> localidadDAO);

    void setUsuario(const Usuario& usuario);
    [[nodiscard]] Localidad obtenerLocalidadActual() const;
    [[nodiscard]] Usuario obtenerUsuario() const;
    void setLocalidadActual(const Localidad& localidad);
    void setLogueado(const bool log) { logueado = log; }
    [[nodiscard]] bool getLogueado() const { return logueado; }

    int getMovimientos() const {return detective.getMovimientos(); }

    //Gestionar reinicio y finalizacion
    void reiniciarJuego();
    void finalizarJuego(GameWindow* gameWindow);

    Localidad obtenerLocalidadAleatoria();
    Secuaz obtenerSecuazAleatorio();
    void marcarSecuazComoCapturado(int secuaz_id); //para gestioanr secuaces atrapados en partida
    bool haySecuacesDisponibles() const;
    void actualizarRango();
    Rango obtenerRangoDetective() const;
    bool todosLosSecuacesCapturados() const;
private:
    Usuario jugador;
    Localidad localidad_actual;
    std::shared_ptr<UsuarioDAO> usuarioDAO;
    std::shared_ptr<SecuazDAO> secuazDAO;
    std::shared_ptr<PistaDAO> pistaDAO;
    std::shared_ptr<LocalidadDAO> localidadDAO;

    Detective detective; // Instancia del detective
    bool logueado = false;
    std::set<int> secuaces_capturados;  // Un conjunto para registrar los IDs de secuaces capturados en esta partida


};

#endif //GAMECONTROLLER_H
