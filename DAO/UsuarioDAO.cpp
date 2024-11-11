#include "UsuarioDAO.h"
#include <stdexcept>

UsuarioDAO::UsuarioDAO(DatabaseHandler& dbHandler) : dbHandler(dbHandler) {
    if (!dbHandler.isConnected()) {
        std::cerr << "DatabaseHandler no está conectado en UsuarioDAO." << std::endl;
    }
}


bool UsuarioDAO::registrarUsuario(const std::string& nombre, const std::string& apellido, const std::string& contrasena) {
    // Preparar la consulta de inserción de datos
    sqlite3_stmt* stmt;
    std::string query = "INSERT INTO usuarios (nombre, apellido, contrasena) VALUES (?, ?, ?)";

    // Preparar la sentencia SQL
    if (sqlite3_prepare_v2(dbHandler.getDb(), query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta para registrar el usuario: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        return false;
    }

    // Asignar valores a los parámetros de la consulta
    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, apellido.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contrasena.c_str(), -1, SQLITE_STATIC);

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error al ejecutar la consulta para registrar el usuario: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        sqlite3_finalize(stmt); // Liberar recursos
        return false;
    }

    sqlite3_finalize(stmt); // Liberar recursos al finalizar
    std::cout << "Usuario registrado exitosamente en la base de datos." << std::endl;
    return true;
}
std::optional<Usuario> UsuarioDAO::autenticarUsuario(const std::string& nombre, const std::string& apellido, const std::string& contrasena) {
    const std::string sql = "SELECT id, nombre, apellido, rango, capturas FROM usuarios WHERE nombre = ? AND apellido = ? AND contrasena = ?;";
    sqlite3_stmt* stmt;
    sqlite3* db = dbHandler.getDb();

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta de autenticación de usuario: " << sqlite3_errmsg(db) << std::endl;
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, apellido.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contrasena.c_str(), -1, SQLITE_STATIC);

    std::optional<Usuario> usuario;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* nombreResultado = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* apellidoResultado = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* rangoResultado = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int capturas = sqlite3_column_int(stmt, 4);

        std::string nombreUsuario = nombreResultado ? nombreResultado : "";
        std::string apellidoUsuario = apellidoResultado ? apellidoResultado : "";
        std::string rangoNombre = rangoResultado ? rangoResultado : "Junior"; // Valor por defecto si rango es nulo

        // Crear el objeto Rango
        Rango rango(rangoNombre);

        // Crear el objeto Usuario con el rango adecuado
        usuario = Usuario(id, nombreUsuario, apellidoUsuario, rango, capturas);
    } else {
        std::cerr << "Usuario no encontrado o credenciales incorrectas." << std::endl;
    }

    sqlite3_finalize(stmt);
    return usuario;
}



bool UsuarioDAO::actualizarRangoUsuario(int idUsuario, const std::string& nuevoRango) {
    sqlite3_stmt* stmt;
    const std::string sql = "UPDATE usuarios SET rango = ? WHERE id = ?";

    if (sqlite3_prepare_v2(dbHandler.getDb(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error al preparar la actualización del rango: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, nuevoRango.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, idUsuario);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error al actualizar el rango del usuario: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    std::cout << "Rango actualizado exitosamente para el usuario ID: " << idUsuario << std::endl;
    return true;
}

bool UsuarioDAO::incrementarCapturas(int usuarioId) {
    sqlite3_stmt* stmt;
    const std::string sql = "UPDATE usuarios SET capturas = capturas + 1 WHERE id = ?";

    if (sqlite3_prepare_v2(dbHandler.getDb(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error al preparar la actualización de capturas: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, usuarioId);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error al actualizar las capturas del usuario: " << sqlite3_errmsg(dbHandler.getDb()) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

