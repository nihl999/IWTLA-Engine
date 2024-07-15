cmake_minimum_required(VERSION 3.29.3)


project(ouroboros_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(main 
    src/main.cpp
    src/core/application/application.cpp
    src/core/graphics/renderer/shader.cpp
    src/core/graphics/renderer/impl/opengl_renderer.cpp
    src/core/resources/resource_system.cpp
    src/core/window/impl/glfw_window.cpp
    src/core/world/scene.cpp
    src/Camera.cpp
    src/Input.cpp
    src/common/stb_image.cpp
    src/common/File/File.cpp
    src/core/utils/uuid/uuid.cpp
)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/) 

target_include_directories(main 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/src/
)

target_link_directories(main
    PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/
)


add_subdirectory(vendor/flecs)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)

#assimp fuckery
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build zlib" FORCE)
SET (ASSIMP_BUILD_TESTS OFF)
SET (ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(vendor/assimp)


target_link_libraries(main 
    flecs::flecs_static
    glfw
    glad
    stb::image
    stb::image_write
    stb::noise
    assimp
    imgui
    -lWs2_32
    -luser32 
    -lgdi32 
    -lopengl32
)






