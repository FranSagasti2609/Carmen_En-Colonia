#include "MisionDAO.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sqlite3.h>


// Método para obtener todas las misiones desde la base de datos
std::vector<Mision> MisionDAO::obtenerTodasLasMisiones() {
    std::vector<Mision> misiones;
    std::string query = "SELECT id, titulo, descripcion, objetivo, completada, usuario_id FROM Misiones";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Obtener los valores de la consulta SQL
            int id = sqlite3_column_int(stmt, 0);
            const char* titulo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* descripcion = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* objetivo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            bool completada = sqlite3_column_int(stmt, 4) == 1;
            int usuario_id = sqlite3_column_int(stmt, 5); // Nueva columna agregada

            // Crear una nueva misión con los datos obtenidos y añadirla al vector
            Mision mision(id, titulo, descripcion, objetivo, completada, usuario_id);
            misiones.push_back(mision);
        }
    } else {
        throw std::runtime_error("Error al ejecutar la consulta SQL para obtener misiones.");
    }

    sqlite3_finalize(stmt);
    return misiones;
}
