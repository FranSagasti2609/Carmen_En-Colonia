#include "SecuazDAO.h"
#include <random>
#include <iostream>

// Constructor del SecuazDAO
SecuazDAO::SecuazDAO(DatabaseHandler& dbHandler) : dbHandler(dbHandler) {}

// Obtener un secuaz aleatorio que no haya sido capturado
std::string SecuazDAO::obtenerSecuazAleatorio() {
    std::string query = "SELECT nombre FROM secuaces WHERE capturado = 0;";
    auto resultados = dbHandler.fetchQuery(query);

    if (!resultados.empty()) {
        // Selecciona un secuaz al azar entre los disponibles
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, resultados.size() - 1);
        return resultados[distrib(gen)][0];
    }
    return "Secuaz desconocido";
}

// Obtener secuaces por localidad
std::vector<Secuaz> SecuazDAO::obtenerSecuacesPorLocalidad(int localidad_id) {
    std::vector<Secuaz> secuaces;
    std::string query = "SELECT id, nombre FROM secuaces WHERE localidad_id = ? AND capturado = 0";
    auto statement = dbHandler.prepararConsulta(query);

    if (statement) {
        dbHandler.bindInt(statement, 1, localidad_id);
        while (dbHandler.ejecutarPaso(statement)) {
            int id = dbHandler.obtenerInt(statement, 0);
            std::string nombre = dbHandler.obtenerString(statement, 1);
            secuaces.emplace_back();
        }
        dbHandler.finalizarConsulta(statement);
    } else {
        std::cerr << "Error al preparar la consulta para obtener secuaces por localidad." << std::endl;
    }

    return secuaces;
}

// Marcar un secuaz como capturado por su ID
bool SecuazDAO::capturarSecuaz(int secuazId) {
    std::string query = "UPDATE secuaces SET capturado = 1 WHERE id = ?;";
    auto statement = dbHandler.prepararConsulta(query);

    if (statement) {
        dbHandler.bindInt(statement, 1, secuazId);
        dbHandler.ejecutarPaso(statement);
        dbHandler.finalizarConsulta(statement);
        return true;
    } else {
        std::cerr << "Error al preparar la consulta para capturar secuaz." << std::endl;
        return false;
    }
}

// Obtener todos los secuaces
std::vector<Secuaz> SecuazDAO::getTodosLosSecuaces() {
    std::vector<Secuaz> secuaces;
    const std::string sql = "SELECT id, nombre, habilidad, peligrosidad, localidad_id, capturado FROM secuaces";
    auto resultados = dbHandler.fetchQuery(sql);
    LocalidadDAO localidadDAO(dbHandler);  // Crear instancia para obtener localidad

    for (const auto& fila : resultados) {
        if (fila.size() < 6) continue;
        int id = std::stoi(fila[0]);
        std::string nombre = fila[1];
        std::string habilidad = fila[2];
        int peligrosidad = std::stoi(fila[3]);
        int localidad_id = (fila[4].empty() || fila[4] == "NULL") ? -1 : std::stoi(fila[4]);
        bool capturado = (fila[5] == "1");

        // Obtener localidad asociada y crear el puntero
        std::shared_ptr<Localidad> localidad = std::make_shared<Localidad>(localidadDAO.getLocalidadPorId(localidad_id));

        // Crear el objeto Secuaz y agregarlo al vector
        secuaces.emplace_back(id, nombre, habilidad, peligrosidad, localidad, capturado);
    }

    return secuaces;
}

// Obtener un secuaz por su ID
Secuaz SecuazDAO::obtenerSecuazPorId(int secuazId) {
    Secuaz secuaz;
    std::string sql = "SELECT id, nombre, habilidad, peligrosidad, localidad_id, capturado FROM secuaces WHERE id = ?;";
    auto statement = dbHandler.prepararConsulta(sql);

    if (statement) {
        dbHandler.bindInt(statement, 1, secuazId);
        if (dbHandler.ejecutarPaso(statement)) {
            int id = dbHandler.obtenerInt(statement, 0);
            std::string nombre = dbHandler.obtenerString(statement, 1);
            std::string habilidad = dbHandler.obtenerString(statement, 2);
            int peligrosidad = dbHandler.obtenerInt(statement, 3);
            int localidad_id = dbHandler.obtenerInt(statement, 4);
            bool capturado = dbHandler.obtenerBool(statement, 5);

            // Obtener localidad y crear el objeto
            LocalidadDAO localidadDAO(dbHandler);
            std::shared_ptr<Localidad> localidad = std::make_shared<Localidad>(localidadDAO.getLocalidadPorId(localidad_id));

            secuaz = Secuaz(id, nombre, habilidad, peligrosidad, localidad, capturado);
        }
        dbHandler.finalizarConsulta(statement);
    } else {
        throw std::runtime_error("Error al preparar la consulta para obtener el secuaz por ID.");
    }

    return secuaz;
}

Secuaz SecuazDAO::obtenerSecuazPorNombre(const std::string& nombreSecuaz) {
    std::string sql = "SELECT id, nombre, habilidad, peligrosidad, localidad_id, capturado FROM secuaces WHERE nombre = ?;";
    auto statement = dbHandler.prepararConsulta(sql);

    Secuaz secuaz;
    if (statement) {
        dbHandler.bindString(statement, 1, nombreSecuaz);
        if (dbHandler.ejecutarPaso(statement)) {
            int id = dbHandler.obtenerInt(statement, 0);
            std::string nombre = dbHandler.obtenerString(statement, 1);
            std::string habilidad = dbHandler.obtenerString(statement, 2);
            int peligrosidad = dbHandler.obtenerInt(statement, 3);
            int localidad_id = dbHandler.obtenerInt(statement, 4);
            bool capturado = dbHandler.obtenerBool(statement, 5);

            // Crear secuaz con los datos
            secuaz = Secuaz(id, nombre, habilidad, peligrosidad, nullptr, capturado);
        }
        dbHandler.finalizarConsulta(statement);
    }
    return secuaz;
}
