cmake_minimum_required(VERSION 3.20)
project(lunaraytracer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE
    main.cpp
    vec3.cpp)

include_directories("C:/Program Files (x86)/Eigen3/include/eigen3")

add_executable(${PROJECT_NAME} ${SOURCE})