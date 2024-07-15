cmake_minimum_required(VERSION 3.29.3)


project(sandbox)

add_executable(sandbox
    src/application.cpp
)

add_compile_definitions(OURO_PLATFORM_WIN)

target_include_directories(sandbox PRIVATE ${CMAKE_SOURCE_DIR}/engine/src)
target_include_directories(sandbox PRIVATE ${CMAKE_SOURCE_DIR}/engine/include)

target_link_libraries(sandbox
${CMAKE_SOURCE_DIR}/engine/bin/engine)

set_target_properties(sandbox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/sandbox/bin/) 