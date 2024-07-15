cmake_minimum_required(VERSION 3.29.3)

project(GLAD)

add_library(glad STATIC
"./include/glad/glad.h"
"./src/glad.c")
target_include_directories(glad PUBLIC include)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

