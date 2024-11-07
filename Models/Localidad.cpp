#include "Localidad.h"

int Localidad::getId() const {
    return id;
}
// Obtener el nombre de la localidad
std::string Localidad::getNombre() const {
    return nombre;
}

// Obtener la descripción de la localidad
std::string Localidad::getDescripcion() const {
    return descripcion;
}

// Obtener la latitud de la localidad
double Localidad::getLatitud() const {
    return latitud;
}

// Obtener la longitud de la localidad
double Localidad::getLongitud() const {
    return longitud;
}

// Asignar un nuevo secuaz a la localidad
void Localidad::asignarSecuaz(const std::string& nuevoSecuaz) {
    secuaz = nuevoSecuaz;
}

// Verificar si el secuaz es "Carmen Sandiego"
bool Localidad::esCarmenSandiego() const {
    return secuaz == "Carmen San Diego";
}
