# Laboratori 2 · Matemàtiques II · 2026

1. Descarregueu el repositori amb **Code → Download ZIP** i descomprimiu-lo.
2. Instal·leu **CMake 3.24 o posterior** i un compilador **C++17**. A Windows,
   instal·leu Visual Studio amb «Desktop development with C++»; a macOS,
   les eines de Xcode; a Linux, les eines de compilació i les dependències
   de desenvolupament d’OpenGL i SDL3.
3. Obriu un terminal a la carpeta del projecte i executeu:

```sh
cmake --preset default
cmake --build --preset default --parallel
```

La primera compilació necessita Internet per descarregar SDL3 i Dear ImGui.

4. Executeu l’aplicació:

**Windows (PowerShell):**

```powershell
.\build\bin\lab2.exe
```

**macOS/Linux:**

```sh
./build/bin/lab2
```

Després de modificar el codi, repetiu la comanda de compilació i torneu a
obrir l’aplicació.
