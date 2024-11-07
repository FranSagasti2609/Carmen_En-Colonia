#ifndef SECUAZDAO_H
#define SECUAZDAO_H

#include "../DatabaseHandler.h"
#include "../Models/Secuaz.h"
#include "LocalidadDAO.h"
#include <string>
#include <vector>
#include <memory>

class SecuazDAO {
public:
    // Constructor
    explicit SecuazDAO(DatabaseHandler& dbHandler);

    // Métodos
    std::string obtenerSecuazAleatorio();  // Obtener un secuaz aleatorio que no haya sido capturado
    std::vector<Secuaz> obtenerSecuacesPorLocalidad(int localidad_id);  // Obtener lista de secuaces en una localidad específica
    Secuaz obtenerSecuazPorNombre(const std::string &nombreSecuaz);
    bool capturarSecuaz(int secuazId);  // Marcar secuaz como capturado por su ID
    std::vector<Secuaz> getTodosLosSecuaces();  // Obtener todos los secuaces
    Secuaz obtenerSecuazPorId(int secuazId);  // Obtener un secuaz por su ID

private:
    DatabaseHandler& dbHandler;  // Manejador de la base de datos
};

#endif // SECUAZDAO_H
