#ifndef PARTIDADAO_H
#define PARTIDADAO_H

#include "BaseDAO.h"
#include "../Models/Partida.h"
#include <vector>

class PartidaDAO : public BaseDAO {
public:
    // Constructor que inicializa el DAO de Partida
    explicit PartidaDAO(sqlite3* db);

    // Método para insertar una nueva partida
    bool insertarPartida(const Partida& partida);

    // Método para obtener todas las partidas de la base de datos
    std::vector<Partida> obtenerPartidas();
};

#endif //PARTIDADAO_H
