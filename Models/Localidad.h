#ifndef LOCALIDAD_H
#define LOCALIDAD_H

#include <string>
#include <iostream>

class Localidad {
public:
    // Constructor que acepta todos los atributos
    Localidad(int id, const std::string& nombre, const std::string& descripcion, double latitud, double longitud)
        : id(id), nombre(nombre), descripcion(descripcion), latitud(latitud), longitud(longitud) {}

    Localidad() : id(0), nombre("NULL_CITY") {}

    // Métodos para obtener el nombre y el secuaz asignado
    std::string getNombre() const;
    std::string getDescripcion() const;  // Método para obtener la descripción
    double getLatitud() const;           // Método para obtener la latitud
    double getLongitud() const;          // Método para obtener la longitud
    int getId() const;

    // Asignar un nuevo secuaz a la localidad
    void asignarSecuaz(const std::string& nuevoSecuaz);

    // Verificar si el secuaz es "Carmen Sandiego"
    bool esCarmenSandiego() const;

private:
    int id;                             // ID de la localidad
    std::string nombre;                 // Nombre de la localidad
    std::string descripcion;            // Descripción de la localidad
    std::string secuaz;
    double latitud;                     // Latitud de la localidad
    double longitud;                    // Longitud de la localidad
};

#endif // LOCALIDAD_H
