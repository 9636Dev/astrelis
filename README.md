# Astrelis
Astrelis is a cross-platform 2D/3D game engine written in C++.

## Building
First, you need to ensure you initialize the submodules:

```
git submodule update --init --recursive
```

Then, you can build Astrelis using CMake:

```
cmake -B build .
```
If you are Windows, you may want to generate the Visual Studio solution:

```
cmake -B build -G "Visual Studio 17 2022" -A x64 .
```

If you generate makefiles or ninjja files, you can use the following command to build:
You can then build Astrelis using the generated build files:

```
cmake --build build
```

You can specify number of threads to use for building:

```
cmake --build build -j4
```

## Running
After building, you need to first run the setup to compile the shaders:

```
cd run
../build/bin/Astrelis_EditorSetup
cd ..
```
On windows, you can run it like this:

```
cd run
..\build\bin\Astrelis_EditorSetup.exe
cd ..
```

Then, you can run the editor:

```
./build/bin/Astrelis_Editor
```

On windows, you can run it like this:
```
.\build\bin\Astrelis_Editor.exe
```
