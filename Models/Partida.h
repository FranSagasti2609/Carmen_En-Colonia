#ifndef PARTIDA_H
#define PARTIDA_H

#include <string>

class Partida {
private:
    int id;
    std::string descripcion; // Nuevo atributo para almacenar la descripción de la partida
    int usuario_id; // Nuevo atributo para almacenar el ID del usuario
    std::string estado;
    int intentos;

public:
    // Constructor
    //Partida(int id, const std::string& descripcion, int usuario_id, const std::string& estado, int intentos);
    // Modificar el constructor en Partida.h
    Partida(int id, const std::string& descripcion, int usuario_id, const std::string& estado = "", int intentos = 0);

    // Getters
    int getId() const;
    std::string getDescripcion() const; // Nuevo getter
    int getUsuarioId() const; // Nuevo getter
    std::string getEstado() const;
    int getIntentos() const;

    // Setters
    void setDescripcion(const std::string& descripcion); // Nuevo setter
    void setUsuarioId(int usuario_id); // Nuevo setter
    void setEstado(const std::string& estado);
    void setIntentos(int intentos);
};

#endif //PARTIDA_H
