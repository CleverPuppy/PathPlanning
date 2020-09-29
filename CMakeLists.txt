cmake_minimum_required(VERSION 3.3)

# set the project name
project(PathPlanning)

# find glfw
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")
# sepecify the c++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

INCLUDE_DIRECTORIES(./include/)


add_executable(${PROJECT_NAME} "main.cpp"
                               "graph.cpp"
                               "robot.cpp"
                               "maprender.cpp"
                               "glad.c"
                               "pathplan.cpp"
                               "qlbase.cpp"
                               "examples.cpp")

target_link_libraries(${PROJECT_NAME} glfw GL GLU)
