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
    const std::string sql = "SELECT id, nombre, apellido FROM usuarios WHERE nombre = ? AND apellido = ? AND contrasena = ?;";
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
        const char* nombreUsuario = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* apellidoUsuario = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        usuario = Usuario(id, nombreUsuario ? nombreUsuario : "", apellidoUsuario ? apellidoUsuario : "");
    } else {
        std::cerr << "Usuario no encontrado o credenciales incorrectas." << std::endl;
    }
    sqlite3_finalize(stmt);
    return usuario;
}