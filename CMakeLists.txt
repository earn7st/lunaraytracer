cmake_minimum_required(VERSION 3.20)
project(lunaraytracer)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

set(SOURCE
    main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})