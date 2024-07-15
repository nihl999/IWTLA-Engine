cmake_minimum_required(VERSION 3.29.3)

project(stb)

add_library(stb_image STATIC image.h image.cpp)
add_library(stb_image_write STATIC image_write.h image_write.cpp)
add_library(stb_noise STATIC noise.h noise.cpp)

add_library(stb::image ALIAS stb_image)
add_library(stb::image_write ALIAS stb_image_write)
add_library(stb::noise ALIAS stb_noise)



option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

