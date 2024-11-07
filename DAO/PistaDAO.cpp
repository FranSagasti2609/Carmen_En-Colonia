#include "PistaDAO.h"
#include <random>
#include <iostream>

// Constructor que inicializa el DAO de pistas con el manejador de base de datos
PistaDAO::PistaDAO(DatabaseHandler& dbHandler) : dbHandler(dbHandler) {}

// Selecciona un ID de localidad aleatorio de la base de datos
int PistaDAO::seleccionarLocalidadAleatoria() {
    int localidadId = -1;
    std::string query = "SELECT id FROM localidades ORDER BY RANDOM() LIMIT 1;";

    auto resultado = dbHandler.fetchQuery(query);
    if (!resultado.empty()) {
        localidadId = std::stoi(resultado[0][0]);
    } else {
        std::cerr << "Error: No se encontró ninguna localidad en la base de datos." << std::endl;
    }
    return localidadId;
}

// Obtiene dos pistas falsas y una correcta para una localidad aleatoria
std::vector<std::string> PistaDAO::obtenerPistasParaLocalidadAleatoria() {
    std::vector<std::string> pistasSeleccionadas;

    // Seleccionar una localidad aleatoria
    int localidadId = seleccionarLocalidadAleatoria();
    if (localidadId == -1) {
        return pistasSeleccionadas; // Retorna vacío si no se selecciona una localidad válida
    }

    // Consulta para obtener las primeras cuatro pistas falsas de la localidad seleccionada
    std::string queryFalsas = "SELECT descripcion FROM pistas WHERE localidad_id = " + std::to_string(localidadId) + " AND esCorrecta = 0 LIMIT 4;";
    auto pistasFalsas = dbHandler.fetchQuery(queryFalsas);

    // Verificar que haya al menos cuatro pistas falsas
    if (pistasFalsas.size() < 4) {
        std::cerr << "Error: No se encontraron suficientes pistas falsas para la localidad " << localidadId << std::endl;
        return pistasSeleccionadas;
    }

    // Seleccionar aleatoriamente dos de las cuatro pistas falsas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 3);

    int indiceFalsa1 = distrib(gen);
    int indiceFalsa2;
    do {
        indiceFalsa2 = distrib(gen);
    } while (indiceFalsa2 == indiceFalsa1);

    pistasSeleccionadas.push_back(pistasFalsas[indiceFalsa1][0]);
    pistasSeleccionadas.push_back(pistasFalsas[indiceFalsa2][0]);

    // Consulta para obtener la pista correcta de la localidad
    std::string queryCorrecta = "SELECT descripcion FROM pistas WHERE localidad_id = " + std::to_string(localidadId) + " AND esCorrecta = 1 LIMIT 1;";
    auto pistaCorrecta = dbHandler.fetchQuery(queryCorrecta);

    // Añadir la pista correcta a las seleccionadas
    if (!pistaCorrecta.empty()) {
        pistasSeleccionadas.push_back(pistaCorrecta[0][0]);
    } else {
        std::cerr << "Error: No se encontró pista correcta para la localidad " << localidadId << std::endl;
    }

    return pistasSeleccionadas;
}

std::vector<Pista> PistaDAO::obtenerPistasPorLocalidad(int localidadId) {
    std::vector<Pista> pistas;

    std::string query = "SELECT id, descripcion, esCorrecta FROM pistas WHERE localidad_id = " + std::to_string(localidadId) + ";";
    auto resultado = dbHandler.fetchQuery(query);

    for (const auto& fila : resultado) {
        if (fila.size() < 3) continue; // Verificar que hay suficientes columnas

        int id = std::stoi(fila[0]);
        std::string descripcion = fila[1];
        bool esCorrecta = (fila[2] == "1");

        // Crear instancia de Pista y añadirla al vector
        pistas.emplace_back(id, descripcion, localidadId, esCorrecta);
    }

    return pistas;
}


std::vector<Pista> PistaDAO::getTodasLasPistas() {
    std::vector<Pista> pistas;

    // Consulta SQL para obtener todas las pistas
    std::string query = "SELECT id, descripcion, localidad_id, esCorrecta FROM pistas;";
    auto resultado = dbHandler.fetchQuery(query);

    // Recorrer cada fila de resultados y crear instancias de Pista
    for (const auto& fila : resultado) {
        if (fila.size() < 4) continue; // Verificar que hay suficientes columnas

        int id = std::stoi(fila[0]);
        std::string descripcion = fila[1];
        int localidadId = std::stoi(fila[2]);
        bool esCorrecta = (fila[3] == "1");

        // Crear instancia de Pista y añadirla al vector
        pistas.emplace_back(id, descripcion, localidadId, esCorrecta);
    }

    return pistas;
}

Pista PistaDAO::obtenerPistaCorrectaPorLocalidad(const Localidad& localidad) {
    // Obtener todas las pistas de la localidad usando su ID
    std::vector<Pista> pistasLocalidad = obtenerPistasPorLocalidad(localidad.getId());
    
    // Iterar sobre las pistas y buscar la correcta
    for (const Pista& pista : pistasLocalidad) { // Cambia a Pista
        if (pista.getEsCorrecta()) { // Verificar si la pista es correcta
            return pista;// Retornar la descripción de la pista correcta
        }
    }

    // Si no se encontró pista correcta, lanzar una excepción o retornar un mensaje
    throw std::runtime_error("No se encontró una pista correcta para la localidad especificada.");
}
