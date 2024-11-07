#ifndef DETECTIVE_H
#define DETECTIVE_H

#include <iostream>
#include "Rango.h"  // Asegúrate de incluir la clase Rango

class Detective {
public:
    // Constructor para inicializar con un número de movimientos
    explicit Detective(int movimientos_iniciales);

    // Constructor default
    Detective();

    // Método para decrementar los movimientos disponibles del detective
    void decrementarMovimientos();

    // Obtener el número de movimientos restantes
    int getMovimientos() const;

    // Restablecer los movimientos a un valor específico
    void resetMovimientos(int nuevos_movimientos);

    // Obtener el rango actual del detective
    Rango getRango() const;

    // Avanzar al siguiente rango
    void avanzarRango();

private:
    int movimientos_;  // Número de movimientos disponibles para el detective
    Rango rango_;      // Rango actual del detective
};

#endif //DETECTIVE_H
