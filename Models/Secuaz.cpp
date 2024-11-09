
#include "Secuaz.h"

// Constructor por defecto
Secuaz::Secuaz() : id(0), nombre(""), habilidad(""), peligrosidad(0), capturado(false) {}

// Constructor parametrizado
Secuaz::Secuaz(int id, const std::string& nombre, const std::string& habilidad,
               int peligrosidad, std::shared_ptr<Localidad> localidad, bool capturado)
    : id(id), nombre(nombre), habilidad(habilidad), peligrosidad(peligrosidad), localidad(localidad), capturado(capturado) {}

// Métodos Getters
int Secuaz::getId() const { return id; }
std::string Secuaz::getNombre() const { return nombre; }
std::string Secuaz::getHabilidad() const { return habilidad; }
int Secuaz::getPeligrosidad() const { return peligrosidad; }
std::shared_ptr<Localidad> Secuaz::getLocalidad() const { return localidad; }
bool Secuaz::isCapturado() const { return capturado; }

// Métodos Setters
void Secuaz::setNombre(const std::string& nombre) { this->nombre = nombre; }
void Secuaz::setHabilidad(const std::string& habilidad) { this->habilidad = habilidad; }
void Secuaz::setPeligrosidad(int peligrosidad) { this->peligrosidad = peligrosidad; }
void Secuaz::setLocalidad(std::shared_ptr<Localidad> localidad) { this->localidad = localidad; }
void Secuaz::setCapturado(bool capturado) { this->capturado = capturado; }

std::string Secuaz::getImagePath() const {
    std::string basePath = "../Multimedia/";

    if (nombre == "Betosecreto") {
        return basePath + "Betosecreto.png";
    } else if (nombre == "EllaBella") {
        return basePath + "EllaBella.png";
    } else if (nombre == "Mindy Ana Son") {
        return basePath + "Mindy Ana Son.png";
    } else if (nombre == "MOONabomber") {
        return basePath + "Moonabomber.png";
    } else if (nombre == "Carmen San Diego") {
        return basePath + "CSD.png";
    }

    return basePath + "unknown.png"; // Imagen por defecto si no se reconoce el secuaz
}


