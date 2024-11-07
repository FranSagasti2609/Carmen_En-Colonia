import os
import subprocess
import shutil
import glob

# Configuración de directorios
project_dir = os.path.abspath("C:/Users/gring/Downloads/FINAL_TEST")
build_dir = os.path.join(project_dir, "build")
install_dir = os.path.abspath("C:/Program Files/TuPrograma")
bin_dir = os.path.join(install_dir, "bin")

# Ruta al ícono y al archivo de recursos
icon_path = os.path.join(project_dir, "Multimedia", "icon.ico")
rc_file_path = os.path.join(project_dir, "resources.rc")

# Crear el archivo de recursos (.rc) si no existe
with open(rc_file_path, "w") as rc_file:
    rc_file.write(f'IDI_ICON1 ICON "{icon_path}"\n')

# Paso 1: Configuración y compilación del proyecto con CMake
try:
    # Generar archivos de construcción con CMake
    subprocess.run(
        ["cmake", "-S", project_dir, "-B", build_dir, "-DCMAKE_BUILD_TYPE=Release"],
        check=True,
    )
    # Compilar el proyecto con mingw32-make
    subprocess.run(
        ["cmake", "--build", build_dir, "--config", "Release"],
        check=True,
    )
except subprocess.CalledProcessError:
    print("Error en la compilación del proyecto.")
    exit(1)

# Paso 2: Copiar todas las dependencias (.dll) de gtkmm4 y sqlite3

# Definir el directorio donde se encuentran las DLLs de las bibliotecas (ajusta esta ruta según tu configuración)
dependencies_dir = "D:/msys64/mingw64/bin"  # Cambia esta ruta según tu instalación de MSYS2

# Crear el directorio "bin" si no existe
os.makedirs(bin_dir, exist_ok=True)

# Copiar todas las DLLs desde dependencies_dir al bin_dir
for dll_path in glob.glob(os.path.join(dependencies_dir, "*.dll")):
    shutil.copy(dll_path, bin_dir)
    print(f"Copiado {os.path.basename(dll_path)} a {bin_dir}")

# Copiar el ejecutable compilado al directorio "bin"
executable_path = os.path.join(build_dir, "FINAL_TEST.exe")
if os.path.exists(executable_path):
    shutil.copy(executable_path, bin_dir)
    print(f"Ejecutable copiado a {bin_dir}")
else:
    print("Error: No se generó el ejecutable.")

print("Compilación y configuración completadas. Todas las dependencias copiadas.")
