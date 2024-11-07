#include "Mision.h"
#include <iostream>

// Constructor
Mision::Mision(int id, const std::string& titulo, const std::string& descripcion,
               const std::string& objetivo, bool completada, int usuario_id)
    : id(id), titulo(titulo), descripcion(descripcion), objetivo(objetivo),
      completada(completada), usuario_id(usuario_id) {}

// Getters
int Mision::getId() const {
    return id;
}

std::string Mision::getTitulo() const {
    return titulo;
}

std::string Mision::getDescripcion() const {
    return descripcion;
}

std::string Mision::getObjetivo() const {
    return objetivo;
}

bool Mision::isCompletada() const {
    return completada;
}

int Mision::getUsuarioId() const {
    return usuario_id;
}

// Setters
void Mision::setId(int newId) {
    id = newId;
}

void Mision::setTitulo(const std::string& newTitulo) {
    titulo = newTitulo;
}

void Mision::setDescripcion(const std::string& newDescripcion) {
    descripcion = newDescripcion;
}

void Mision::setObjetivo(const std::string& newObjetivo) {
    objetivo = newObjetivo;
}

void Mision::setCompletada(bool estado) {
    completada = estado;
}

void Mision::setUsuarioId(int newUsuarioId) {
    usuario_id = newUsuarioId;
}

// Método para mostrar detalles de la misión
void Mision::mostrarDetalles() const {
    std::cout << "Misión: " << titulo << "\n"
              << "Descripción: " << descripcion << "\n"
              << "Objetivo: " << objetivo << "\n"
              << "Completada: " << (completada ? "Sí" : "No") << "\n"
              << "ID de usuario: " << usuario_id << "\n";
}