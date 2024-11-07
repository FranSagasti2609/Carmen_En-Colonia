#ifndef BASEDAO_H
#define BASEDAO_H

#include <sqlite3.h>
#include <string>
#include <stdexcept>

class BaseDAO {
public:
    // Constructor que inicializa la conexión a la base de datos
    explicit BaseDAO(sqlite3* database);

    // Destructor que cierra la conexión si está abierta
    ~BaseDAO();

    // Método para ejecutar una consulta SQL sin parámetros
    bool ejecutarSQL(const std::string& consulta);

    // Método para preparar una sentencia SQL con parámetros
    bool prepararStatement(const std::string& consulta, sqlite3_stmt** stmt);

    // Método para cerrar una sentencia SQL
    void cerrarStatement(sqlite3_stmt* stmt);

    // Método para insertar datos iniciales en la base de datos
    bool insertarDatosIniciales();

    // Método para desconectar la base de datos
    void desconectar();


protected: // Cambiado a protected
    sqlite3* db; // Puntero a la base de datos
};

#endif // BASEDAO_H
