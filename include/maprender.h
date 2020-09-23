#ifndef MAPRENDER_H
#define MAPRENDER_H
//#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>
#include <thread>

#include "graph.h"

class MapRender
{
private:
    GLFWwindow *window;
    std::thread render_thread;
public:
    MapRender();
    ~MapRender();

    void updateWindow();

    /*
     * render function
    */
    static void renderMap(GLFWwindow *window);
    /*
     * glfw call back functions
    */
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow* window);
    /*
     * rendering functions
    */
    static void renderStaticMap(const GridMap& static_map);
};

#endif // MAPRENDER_H
