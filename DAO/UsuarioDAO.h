#ifndef USUARIODAO_H
#define USUARIODAO_H

#include <string>
#include "../DataBaseHandler.h"
#include "../Models/Usuario.h"
#include <optional>

class UsuarioDAO {
public:
    // Constructor
    explicit UsuarioDAO(DatabaseHandler& dbHandler);

    // Método para registrar un usuario
    bool registrarUsuario(const std::string& nombre, const std::string& apellido, const std::string& contrasena);
    std::optional<Usuario> autenticarUsuario(const std::string& nombre,const std::string& apellido, const std::string& contrasena);

private:
    DatabaseHandler& dbHandler;
};

#endif // USUARIO_DAO_HPP