#include "LocalidadDAO.h"
#include <iostream>
#include <stdexcept>

// Constructor
LocalidadDAO::LocalidadDAO(DatabaseHandler& dbHandler) : dbHandler(dbHandler) {}

// Implementación del método para obtener todas las localidades
std::vector<Localidad> LocalidadDAO::getTodasLasLocalidades() {
    std::vector<Localidad> localidades;

    std::string sql = "SELECT id, nombre, descripcion, latitud, longitud FROM localidades;";
    auto statement = dbHandler.prepararConsulta(sql);

    if (statement) {
        while (dbHandler.ejecutarPaso(statement)) {
            int id = dbHandler.obtenerInt(statement, 0); // ID en la primera columna
            std::string nombre = dbHandler.obtenerString(statement, 1); // Nombre en la segunda columna
            std::string descripcion = dbHandler.obtenerString(statement, 2); // Descripción en la tercera columna
            double latitud = dbHandler.obtenerDouble(statement, 3); // Latitud en la cuarta columna
            double longitud = dbHandler.obtenerDouble(statement, 4); // Longitud en la quinta columna

            localidades.emplace_back(id, nombre, descripcion, latitud, longitud); // Constructor de Localidad
        }
        dbHandler.finalizarConsulta(statement);
    } else {
        throw std::runtime_error("Error al preparar la consulta para obtener todas las localidades.");
    }

    return localidades;
}

// Implementación del método para obtener una localidad específica por ID
Localidad LocalidadDAO::getLocalidadPorId(int id) {
    Localidad localidad(0, "", "", 0.0, 0.0); // Inicialización con valores predeterminados
    std::string sql = "SELECT nombre, descripcion, latitud, longitud FROM localidades WHERE id = ?;";
    auto statement = dbHandler.prepararConsulta(sql);

    if (statement) {
        dbHandler.bindInt(statement, 1, id); // Vincula el ID a la consulta
        if (dbHandler.ejecutarPaso(statement)) {
            std::string nombre = dbHandler.obtenerString(statement, 0); // Nombre en la primera columna
            std::string descripcion = dbHandler.obtenerString(statement, 1); // Descripción en la segunda columna
            double latitud = dbHandler.obtenerDouble(statement, 2); // Latitud en la tercera columna
            double longitud = dbHandler.obtenerDouble(statement, 3); // Longitud en la cuarta columna

            localidad = Localidad(id, nombre, descripcion, latitud, longitud); // Crear objeto Localidad
        }
        dbHandler.finalizarConsulta(statement);
    } else {
        throw std::runtime_error("Error al preparar la consulta para obtener la localidad por ID.");
    }

    return localidad;
}
