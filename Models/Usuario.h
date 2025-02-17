#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <sqlite3.h>
#include "Rango.h" // Incluye la clase Rango

class Usuario {
private:
    int id;
    std::string nombre;
    std::string apellido;
    std::string contrasena;
    Rango rango; // Cambia el tipo de rango a objeto Rango
    int capturas;

public:
    // Constructor por defecto, inicializa el rango como un objeto Rango
    Usuario() : id(0), capturas(0), rango("Junior") {} // Inicializa rango en "Junior"

    Usuario(int id, const std::string& nombre, const std::string& apellido, int capturas)
       : id(id), nombre(nombre), apellido(apellido), capturas(capturas) {}

    // Constructor para usuarios con rango ya existente
    Usuario(int id, const std::string& nombre, const std::string& apellido, const Rango& rango, int capturas = 0)
        : id(id), nombre(nombre), apellido(apellido), rango(rango), capturas(capturas) {}

    Usuario(int id, const std::string& nombre, const std::string& apellido);
    Usuario(int id, const std::string& nombre, const std::string& apellido, const Rango& rango);

    // Getters
    int getId() const { return id; }
    const Rango& getRango() const { return rango; } // Devuelve el objeto Rango
    int getCapturas() const { return capturas; }
    const std::string& getNombre() const { return nombre; }
    const std::string& getApellido() const { return apellido; }
    void incrementarCapturas() { ++capturas; }

    // Setters
    void setRango(const Rango& newRango) { rango = newRango; } // Acepta un objeto Rango
    void setCapturas(int newCapturas) { capturas = newCapturas; }
    void setNombre(const std::string& nom) { nombre = nom; }
    void setApellido(const std::string& apellido) { this->apellido = apellido; }

    // Método para obtener la imagen según el rango
    std::string getImagePath() const {
        return rango.getImagePath(); // Obtiene la imagen directamente del objeto Rango
    }

    // Método para avanzar al siguiente rango
    void avanzarRango() {
        rango.avanzarRango(); // Llama al método avanzarRango del objeto Rango
    }

    void guardarEnBD(sqlite3* db);
};

#endif // USUARIO_H
