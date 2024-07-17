cmake_minimum_required(VERSION 3.29.3)


project(ouroboros_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
add_compile_definitions(OURO_EXPORT_API OURO_PLATFORM_WIN)

add_library(engine
    SHARED
    src/core/logger.cpp
    src/core/application/application.cpp
    src/core/graphics/renderer/shader.cpp
    src/core/graphics/renderer/impl/opengl_renderer.cpp
    src/core/resources/resource_system.cpp
    src/core/window/impl/glfw_window.cpp
    src/core/world/scene.cpp
    src/core/world/camera.cpp
    src/core/window/input.cpp
    src/core/utils/uuid/uuid.cpp
)

set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/engine/bin/) 
set_target_properties(engine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/engine/bin/lib) 

target_include_directories(engine 
    PRIVATE
    include/
    src/
)

add_subdirectory(vendor/flecs)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/spdlog)

#assimp fuckery
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build zlib" FORCE)
SET (ASSIMP_BUILD_TESTS OFF)
SET (ASSIMP_WARNINGS_AS_ERRORS OFF)
add_subdirectory(vendor/assimp)

#nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(vendor/json)


target_link_libraries(engine 
    flecs::flecs_static
    glfw
    glad
    stb::image
    stb::image_write
    stb::noise
    assimp
    imgui
    nlohmann_json
    spdlog::spdlog
    -lWs2_32
    -luser32 
    -lgdi32 
    -lopengl32
)

add_custom_command(TARGET engine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:engine> "${CMAKE_SOURCE_DIR}/sandbox/bin/"
    COMMENT "Copy dll file to ${CMAKE_SOURCE_DIR}/sandbox/bin/ directory" VERBATIM
)





