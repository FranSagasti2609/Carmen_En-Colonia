#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>

// Clase DatabaseHandler
class DatabaseHandler {
public:
    // Constructor y destructor
    explicit DatabaseHandler(const std::string& db_name);
    DatabaseHandler();
    ~DatabaseHandler();

    // Métodos para ejecutar consultas y obtener datos
    void executeQuery(const std::string& query);
    std::vector<std::vector<std::string>> fetchQuery(const std::string& query);

    // Métodos auxiliares para manejo de consultas
    sqlite3_stmt* prepararConsulta(const std::string& query);
    bool ejecutarPaso(sqlite3_stmt* stmt);
    int obtenerInt(sqlite3_stmt* stmt, int columna);
    std::string obtenerString(sqlite3_stmt* stmt, int columna);
    double obtenerDouble(sqlite3_stmt* stmt, int columna);
    bool obtenerBool(sqlite3_stmt* stmt, int columna); // Agrega esta línea
    void bindInt(sqlite3_stmt* stmt, int indice, int valor);
    void finalizarConsulta(sqlite3_stmt* stmt);
    void bindString(sqlite3_stmt* stmt, int indice, const std::string& valor);
    // Acceso a la base de datos
    sqlite3* getDb();
    std::string getDbName() const;
    bool isConnected() const;
private:
    sqlite3* db = nullptr;
    std::string dbName;
};


#endif // DBHANDLER_H
