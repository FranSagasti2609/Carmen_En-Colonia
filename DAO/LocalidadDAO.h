#ifndef LOCALIDADDAO_H
#define LOCALIDADDAO_H

#include <vector>
#include "../Models/Localidad.h"
#include "../DataBaseHandler.h"

class LocalidadDAO {
public:
    // Constructor que recibe la instancia de DatabaseHandler
    explicit LocalidadDAO(DatabaseHandler& dbHandler);

    // Método para obtener todas las localidades
    std::vector<Localidad> getTodasLasLocalidades();

    // Método para obtener una localidad específica por su ID
    Localidad getLocalidadPorId(int id);

private:
    DatabaseHandler& dbHandler; // Referencia a DatabaseHandler
};

#endif // LOCALIDADDAO_H
