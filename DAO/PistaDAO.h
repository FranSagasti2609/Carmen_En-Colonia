#ifndef PISTADAO_H
#define PISTADAO_H

#include "../DatabaseHandler.h"
#include <string>
#include <vector>
#include "../Models/Pista.h"
#include "../Models/Localidad.h"

class PistaDAO {
public:
    // Constructor
    PistaDAO(DatabaseHandler& dbHandler);

    // Obtiene dos pistas falsas y una correcta para una localidad seleccionada al azar
    std::vector<std::string> obtenerPistasParaLocalidadAleatoria();

    // Método adicional para obtener todas las pistas de una localidad específica
    std::vector<Pista> obtenerPistasPorLocalidad(int localidadId);

    //Metoo para obtener todas las pistas
    std::vector<Pista> getTodasLasPistas();

    Pista obtenerPistaCorrectaPorLocalidad(const Localidad &localidad);
private:
    DatabaseHandler& dbHandler;

    // Función para seleccionar una localidad aleatoria
    int seleccionarLocalidadAleatoria();
};


#endif // PISTADAO_H
