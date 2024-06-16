# g++ -g src/common/stb_image.cpp src/Scene.cpp src/Input.cpp src/core/resources/resource_manager.cpp src/core/graphics/mesh.cpp src/core/graphics/utils/cube_model.cpp src/Application.cpp src/Camera.cpp src/Shader.cpp src/common/File/File.cpp src/main.cpp -o bin/main.exe -Iinclude -Isrc -Llib -lglfw3 -lglad -luser32 -lgdi32 -lopengl32 -limguiglgl -limgui -lassimp -D_DEBUG -std=c++20

#kohi 002
set echo on

outName="main.exe"
cd src
cppFilenames=$(find . -type f -name "*.cpp")
compilerFlags="-g -std=c++20"

includeFlags="-I../src -I../include"
linkerFlags="-L../lib -lglfw3 -lglad -luser32 -lgdi32 -lopengl32 -limguiglgl -limgui -lassimp"

g++ $cppFilenames $compilerFlags -o ../bin/$outName $includeFlags $linkerFlags