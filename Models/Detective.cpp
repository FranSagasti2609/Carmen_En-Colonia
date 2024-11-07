#include "Detective.h"

Detective::Detective(int movimientos_iniciales) : movimientos_(movimientos_iniciales), rango_() {}

Detective::Detective() : movimientos_(0), rango_() {}

void Detective::decrementarMovimientos() {
    if (movimientos_ > 0) {
        movimientos_--;
    }
}

int Detective::getMovimientos() const {
    return movimientos_;
}

void Detective::resetMovimientos(int nuevos_movimientos) {
    movimientos_ = nuevos_movimientos;
}

Rango Detective::getRango() const {
    return rango_;
}

void Detective::avanzarRango() {
    rango_.avanzarRango();
}
