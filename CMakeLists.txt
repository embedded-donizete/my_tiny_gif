cmake_minimum_required(VERSION 3.5.0)
project(my_tiny_gif VERSION 0.1.0 LANGUAGES C)

add_library(my_tiny_gif my_tiny_gif.c)
target_include_directories(my_tiny_gif PUBLIC include)

add_executable(main main.c)
target_link_libraries(main my_tiny_gif)