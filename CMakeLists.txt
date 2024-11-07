cmake_minimum_required(VERSION 3.29)
project(FINAL_TEST)

set(CMAKE_CXX_STANDARD 20)

# Agregar todos los archivos fuente al ejecutable
add_executable(FINAL_TEST main.cpp
        Models/Usuario.cpp
        Models/Usuario.h
        Models/Localidad.cpp
        Models/Localidad.h
        Models/Mision.cpp
        Models/Mision.h
        Models/Secuaz.cpp
        Models/Secuaz.h
        Models/Rango.cpp
        Models/Rango.h
        Models/Pista.cpp
        Models/Pista.h
        Models/Detective.cpp
        Models/Detective.h
        DAO/UsuarioDAO.cpp
        DAO/UsuarioDAO.h
        DataBaseHandler.cpp
        DataBaseHandler.h
        DAO/BaseDAO.cpp
        DAO/BaseDAO.h
        DAO/PistaDAO.cpp
        DAO/PistaDAO.h
        DAO/LocalidadDAO.cpp
        DAO/LocalidadDAO.h
        DAO/MisionDAO.cpp
        DAO/MisionDAO.h
        DAO/PartidaDAO.cpp
        DAO/PartidaDAO.h
        Models/Partida.cpp
        Models/Partida.h
        DAO/SecuazDAO.cpp
        DAO/SecuazDAO.h
        ui/RegistrationWindow.cpp
        ui/RegistrationWindow.h
        Controller/GameController.cpp
        Controller/GameController.h
        ui/CarouselWindow.cpp
        ui/CarouselWindow.h
        ui/GameWindow.cpp
        ui/GameWindow.h
        Controller/Globals.cpp
        Controller/Globals.h
)

# Configuración para GTKmm
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

# Configuración para SQLite3
set(SQLite3_INCLUDE_DIR "D:/msys64/mingw64/include")
set(SQLite3_LIBRARY "D:/msys64/mingw64/lib/libsqlite3.dll.a")
find_library(SQLite3_LIBRARY sqlite3 HINTS "C:/msys64/mingw64/lib")
target_link_libraries(FINAL_TEST ${GTKMM_LIBRARIES} sqlite3)


# Incluir directorios y vincular las bibliotecas encontradas
include_directories(${SQLite3_INCLUDE_DIR})
target_link_libraries(FINAL_TEST ${GTKMM_LIBRARIES} ${SQLite3_LIBRARY})

