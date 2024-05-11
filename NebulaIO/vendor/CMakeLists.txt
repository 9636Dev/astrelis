cmake_minimum_required(VERSION 3.27)

project(STB)

add_library(STBImage INTERFACE stb/stb_image.h)
target_include_directories(STBImage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)
