#include "DatabaseHandler.h"
#include <iostream>

DatabaseHandler::DatabaseHandler(const std::string& db_name) : dbName(db_name) {
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

DatabaseHandler::~DatabaseHandler() {
    if (db) {
        sqlite3_close(db);
    }
}

void DatabaseHandler::executeQuery(const std::string& query) {
    if (!db) {
        std::cerr << "Base de datos no inicializada." << std::endl;
        return;
    }
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error al ejecutar la consulta: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}


std::vector<std::vector<std::string>> DatabaseHandler::fetchQuery(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt* stmt = prepararConsulta(query);

    if (!stmt) { // Verifica si prepararConsulta falló
        std::cerr << "Consulta fallida: " << query << std::endl;
        return results;
    }

    while (ejecutarPaso(stmt)) {
        int numColumns = sqlite3_column_count(stmt);
        std::vector<std::string> row;
        for (int i = 0; i < numColumns; ++i) {
            row.push_back(obtenerString(stmt, i));
        }
        results.push_back(row);
    }
    finalizarConsulta(stmt);
    return results;
}


sqlite3_stmt* DatabaseHandler::prepararConsulta(const std::string& query) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error al preparar consulta: " << sqlite3_errmsg(db) << std::endl;
    }
    return stmt;
}

bool DatabaseHandler::ejecutarPaso(sqlite3_stmt* stmt) {
    return sqlite3_step(stmt) == SQLITE_ROW;
}

int DatabaseHandler::obtenerInt(sqlite3_stmt* stmt, int columna) {
    return sqlite3_column_int(stmt, columna);
}

std::string DatabaseHandler::obtenerString(sqlite3_stmt* stmt, int columna) {
    const unsigned char* texto = sqlite3_column_text(stmt, columna);
    return texto ? reinterpret_cast<const char*>(texto) : "";
}

double DatabaseHandler::obtenerDouble(sqlite3_stmt* stmt, int columna) {
    return sqlite3_column_double(stmt, columna);
}


void DatabaseHandler::finalizarConsulta(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}



sqlite3* DatabaseHandler::getDb() {
    return db;
}

std::string DatabaseHandler::getDbName() const {
    return dbName;
}
void DatabaseHandler::bindInt(sqlite3_stmt* stmt, int indice, int valor) {
    if (!stmt) {
        std::cerr << "Statement nulo en bindInt." << std::endl;
        return;
    }
    if (sqlite3_bind_int(stmt, indice, valor) != SQLITE_OK) {
        std::cerr << "Error al vincular el entero: " << sqlite3_errmsg(db) << std::endl;
    }
}

void DatabaseHandler::bindString(sqlite3_stmt* stmt, int indice, const std::string& valor) {
    if (!stmt) {
        std::cerr << "Statement nulo en bindString." << std::endl;
        return;
    }
    if (sqlite3_bind_text(stmt, indice, valor.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Error al vincular la cadena: " << sqlite3_errmsg(db) << std::endl;
    }
}

bool DatabaseHandler::obtenerBool(sqlite3_stmt* stmt, int columna) {
    int value = sqlite3_column_int(stmt, columna); // Obtener el valor entero
    return value != 0; // Devuelve true si el valor es diferente de 0, de lo contrario false
}

bool DatabaseHandler::isConnected() const {
    return db != nullptr;
}

