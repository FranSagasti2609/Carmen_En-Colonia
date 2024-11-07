#include "Partida.h"

// Constructor
Partida::Partida(int id, const std::string& descripcion, int usuario_id, const std::string& estado, int intentos)
    : id(id), descripcion(descripcion), usuario_id(usuario_id), estado(estado), intentos(intentos) {}

// Getters
int Partida::getId() const {
    return id;
}

std::string Partida::getDescripcion() const {
    return descripcion;
}

int Partida::getUsuarioId() const {
    return usuario_id;
}

std::string Partida::getEstado() const {
    return estado;
}

int Partida::getIntentos() const {
    return intentos;
}

// Setters
void Partida::setDescripcion(const std::string& descripcion) {
    this->descripcion = descripcion;
}

void Partida::setUsuarioId(int usuario_id) {
    this->usuario_id = usuario_id;
}

void Partida::setEstado(const std::string& estado) {
    this->estado = estado;
}

void Partida::setIntentos(int intentos) {
    this->intentos = intentos;
}
