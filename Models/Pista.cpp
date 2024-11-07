#include "Pista.h"

// Constructor completo
Pista::Pista(int id, int localidad_id, int numero, const std::string& descripcion, bool esCorrecta)
    : id(id), localidad_id(localidad_id), numero(numero), descripcion(descripcion), esCorrecta(esCorrecta) {}

// Constructor simplificado (solo id y descripcion)
Pista::Pista(int id, const std::string& descripcion)
    : id(id), descripcion(descripcion), localidad_id(0), numero(0), esCorrecta(false) {}

// Constructor simplificado con localidadId
Pista::Pista(int id, const std::string& descripcion, int localidadId, bool esCorrecta)
    : id(id), descripcion(descripcion), localidad_id(localidadId), esCorrecta(esCorrecta) {}

// Getters
int Pista::getId() const { return id; }
int Pista::getLocalidadId() const { return localidad_id; }
int Pista::getNumero() const { return numero; }
std::string Pista::getDescripcion() const { return descripcion; }
bool Pista::getEsCorrecta() const { return esCorrecta; }

// Setters
void Pista::setId(int newId) { id = newId; }
void Pista::setLocalidadId(int newLocalidadId) { localidad_id = newLocalidadId; }
void Pista::setNumero(int newNumero) { numero = newNumero; }
void Pista::setDescripcion(const std::string& newDescripcion) { descripcion = newDescripcion; }
void Pista::setEsCorrecta(bool newEsCorrecta) { esCorrecta = newEsCorrecta; }
