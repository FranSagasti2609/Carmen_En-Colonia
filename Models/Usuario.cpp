#include "Usuario.h"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

Usuario::Usuario(int id, const std::string& nombre, const std::string& apellido)
    : id(id), nombre(nombre), apellido(apellido) {
    // Inicialización adicional si es necesaria
}

Usuario::Usuario(int id, const std::string& nombre, const std::string& apellido, const Rango& rango)
    : id(id), nombre(nombre), apellido(apellido), rango(rango) {
    // Inicialización adicional si es necesaria
}

// Implementación del método para guardar el usuario en la base de datos
void Usuario::guardarEnBD(sqlite3* db) {
    const char* sql = "INSERT INTO usuarios (nombre, apellido, contrasena, rango, capturas) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, apellido.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, contrasena.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, rango.getNombre().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, capturas);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            throw std::runtime_error("Error al ejecutar la consulta para guardar el usuario.");
        }
        sqlite3_finalize(stmt);
    } else {
        throw std::runtime_error("Error al preparar la declaración SQL para guardar el usuario.");
    }
}
