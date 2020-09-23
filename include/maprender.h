#ifndef MAPRENDER_H
#define MAPRENDER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>

class MapRender
{
private:
    GLFWwindow *window;
public:
    MapRender();
    ~MapRender();

    void render();



    /*
     * glfw call back functions
    */

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif // MAPRENDER_H
