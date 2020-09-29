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
#include "robot.h"

class MapRender
{
private:
    GLFWwindow *window;
    std::thread render_thread;

    static const int gap = 25;
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
public:
    static void render(const MultiAGVManager& manager);
    static void drwaRect(unsigned int x, unsigned int y);
    static void render(const GridMap& static_map);
    static void render(const GroundAGV& rb);
};

#endif // MAPRENDER_H
