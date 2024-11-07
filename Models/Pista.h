#ifndef PISTA_H
#define PISTA_H

#include <string>

class Pista {
private:
    int id;
    int localidad_id;
    int numero;
    std::string descripcion;
    bool esCorrecta;

public:
    // Constructor completo
    Pista(int id, int localidad_id, int numero, const std::string& descripcion, bool esCorrecta);

    // Constructor simplificado (solo id y descripcion)
    Pista(int id, const std::string& descripcion);

    // Constructor simplificado con localidadId
    Pista(int id, const std::string& descripcion, int localidadId, bool esCorrecta);

    //Default
    Pista(){};
    // Getters
    int getId() const;
    int getLocalidadId() const;
    int getNumero() const;
    std::string getDescripcion() const;
    bool getEsCorrecta() const;

    // Setters
    void setId(int newId);
    void setLocalidadId(int newLocalidadId);
    void setNumero(int newNumero);
    void setDescripcion(const std::string& newDescripcion);
    void setEsCorrecta(bool newEsCorrecta);
};

#endif // PISTA_H
