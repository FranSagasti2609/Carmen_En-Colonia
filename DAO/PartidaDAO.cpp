#include "PartidaDAO.h"
#include <stdexcept>
#include <iostream>

// Constructor que inicializa la conexión a la base de datos
PartidaDAO::PartidaDAO(sqlite3* db) : BaseDAO(db) {}

// Método para insertar una nueva partida
bool PartidaDAO::insertarPartida(const Partida& partida) {
    const char* sql = "INSERT INTO partidas (id, descripcion, usuario_id) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, partida.getId());
        sqlite3_bind_text(stmt, 2, partida.getDescripcion().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, partida.getUsuarioId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Error al insertar la partida en la base de datos.");
        }
        sqlite3_finalize(stmt);
        return true; // Partida insertada exitosamente
    } else {
        throw std::runtime_error("Error al preparar la consulta para insertar la partida.");
    }
}

// Método para obtener todas las partidas
std::vector<Partida> PartidaDAO::obtenerPartidas() {
    const char* sql = "SELECT id, descripcion, usuario_id FROM partidas";
    sqlite3_stmt* stmt;
    std::vector<Partida> partidas;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Partida partida(sqlite3_column_int(stmt, 0),
                            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                            sqlite3_column_int(stmt, 2));
            partidas.push_back(partida);
        }
        sqlite3_finalize(stmt);
    } else {
        throw std::runtime_error("Error al preparar la consulta para obtener las partidas.");
    }

    return partidas; // Retornando todas las partidas obtenidas
}

