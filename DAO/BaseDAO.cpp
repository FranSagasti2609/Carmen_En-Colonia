#include "BaseDAO.h"

// Constructor que inicializa la conexión a la base de datos
BaseDAO::BaseDAO(sqlite3* database) : db(database) {
    if (!db) {
        throw std::runtime_error("Base de datos no inicializada.");
    }
}

// Destructor que cierra la conexión si está abierta
BaseDAO::~BaseDAO() {
    desconectar();
}

// Método para ejecutar una consulta SQL sin parámetros
bool BaseDAO::ejecutarSQL(const std::string& consulta) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, consulta.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = errMsg ? errMsg : "Error ejecutando SQL";
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
    return true;
}

// Método para preparar una sentencia SQL con parámetros
bool BaseDAO::prepararStatement(const std::string& consulta, sqlite3_stmt** stmt) {
    if (sqlite3_prepare_v2(db, consulta.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Error preparando sentencia SQL: " + std::string(sqlite3_errmsg(db)));
    }
    return true;
}

// Método para cerrar una sentencia SQL
void BaseDAO::cerrarStatement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

// Método para insertar datos iniciales en la base de datos
bool BaseDAO::insertarDatosIniciales() {
    std::string sql = "INSERT INTO tabla (columna1, columna2) VALUES ('valor1', 'valor2')";
    return ejecutarSQL(sql);
}

// Método para desconectar la base de datos
void BaseDAO::desconectar() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

