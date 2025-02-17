#include "Rango.h"

// Constructor por defecto: empieza en "Junior"
Rango::Rango() : nombre("Junior") {}

// Constructor con nombre específico
Rango::Rango(const std::string& nombre) : nombre(nombre) {}

// Destructor
Rango::~Rango() {}

// Retorna el nombre del rango
std::string Rango::getNombre() const {
    return nombre;
}

// Retorna la ruta de la imagen según el nombre del rango
std::string Rango::getImagePath() const {
    std::string basePath = "../Multimedia/";

    if (nombre == "Junior"|| nombre == "DETECTIVE JUNIOR") {
        return basePath + "DetectiveJunior.png";
    } else if (nombre == "Eficiente") {
        return basePath + "DetectiveEficiente.png";
    } else if (nombre == "Experto") {
        return basePath + "DetectiveExperto.png";
    } else if (nombre == "Senior") {
        return basePath + "DetectiveSenior.png";
    }

    return "Imagen no encontrada.";  // Devuelve una cadena vacía si no hay imagen disponible
}

// Método para avanzar al siguiente rango en el orden
void Rango::avanzarRango() {
    if (nombre == "Junior" || nombre == "DETECTIVE JUNIOR") {
        nombre = "Eficiente";
    } else if (nombre == "Eficiente") {
        nombre = "Experto";
    } else if (nombre == "Experto") {
        nombre = "Senior";
    }
    // No avanza más si el rango es "Senior"
}
