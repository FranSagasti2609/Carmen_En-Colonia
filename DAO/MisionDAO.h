#ifndef MISIONDAO_H
#define MISIONDAO_H

#include <sqlite3.h>
#include <vector>
#include <memory>
#include "../DAO/BaseDAO.h"
#include "../Models/Mision.h"

class MisionDAO : public BaseDAO {
public:
    MisionDAO(sqlite3* db);

    bool crearTabla();
    bool insertarMision(const Mision& mision);
    bool actualizarMision(const Mision& mision);
    bool eliminarMision(int id);
    std::shared_ptr<Mision> obtenerMisionPorId(int id);
    std::vector<Mision> obtenerTodasLasMisiones();
};

#endif //MISIONDAO_H
