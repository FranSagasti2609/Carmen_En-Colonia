#ifndef SECUAZ_H
#define SECUAZ_H

#include "Localidad.h"
#include <memory> //para gestionar punteros

class Secuaz {
private:
    int id;                            // Identificador del secuaz
    std::string nombre;                 // Nombre del secuaz
    std::string habilidad;              // Habilidad especial del secuaz
    int peligrosidad;                   // Nivel de peligrosidad del secuaz
    std::shared_ptr<Localidad> localidad; // Localidad asociada al secuaz
    bool capturado;                     // Estado de captura del secuaz

public:
    // Constructor por defecto
    Secuaz();

    // Constructor parametrizado para inicializar secuaz con detalles completos
    Secuaz(int id, const std::string& nombre, const std::string& habilidad,
           int peligrosidad, std::shared_ptr<Localidad> localidad, bool capturado);

    // Métodos Getters
    int getId() const;
    std::string getNombre() const;
    std::string getHabilidad() const;
    int getPeligrosidad() const;
    std::shared_ptr<Localidad> getLocalidad() const;
    bool isCapturado() const;

    // Métodos Setters
    void setNombre(const std::string& nombre);
    void setHabilidad(const std::string& habilidad);
    void setPeligrosidad(int peligrosidad);
    void setLocalidad(std::shared_ptr<Localidad> localidad);
    void setCapturado(bool capturado);

    std::string getImagePath() const;
};


#endif //SECUAZ_H
