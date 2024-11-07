#ifndef RANGO_H
#define RANGO_H

#include <string>

class Rango {
private:
    std::string nombre; // Nombre del rango

public:
    // Constructores
    Rango(); // Constructor por defecto
    Rango(const std::string& nombre); // Constructor con nombre específico
    ~Rango(); // Destructor

    // Métodos
    std::string getNombre() const; // Obtener el nombre del rango
    std::string getImagePath() const; // Obtener la ruta de la imagen según el nombre del rango
    void avanzarRango(); // Avanzar al siguiente rango en el orden
};

#endif // RANGO_H
