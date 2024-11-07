#ifndef MISION_H
#define MISION_H

#include <string>

class Mision {
private:
    int id;
    std::string titulo;
    std::string descripcion;
    std::string objetivo;
    bool completada;
    int usuario_id;

public:
    // Constructor
    Mision(int id, const std::string& titulo, const std::string& descripcion,
           const std::string& objetivo, bool completada, int usuario_id);

    // Getters
    int getId() const;
    std::string getTitulo() const;
    std::string getDescripcion() const;
    std::string getObjetivo() const;
    bool isCompletada() const;
    int getUsuarioId() const;

    // Setters
    void setId(int newId);
    void setTitulo(const std::string& newTitulo);
    void setDescripcion(const std::string& newDescripcion);
    void setObjetivo(const std::string& newObjetivo);
    void setCompletada(bool estado);
    void setUsuarioId(int newUsuarioId);

    void mostrarDetalles() const;
};

#endif // MISION_H
