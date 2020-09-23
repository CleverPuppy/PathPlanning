#include "maprender.h"
#include "errno.h"
#include <cstring>
#include "unistd.h"
#include <thread>


MapRender::MapRender()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(800,600,"HelloWindow", NULL,NULL);
    if(window == nullptr){
        fprintf(stderr,"glfwCreateWindow failed! errno:%s\n",strerror(errno));
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    /*
     * init opengl
    */
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        exit(-1);
//    }
    glViewport(0,0,800,600);
}

MapRender::~MapRender()
{
    glfwSetWindowShouldClose(window,true);
    glfwPollEvents();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void MapRender::updateWindow()
{
    glfwSwapBuffers(window);
}

void MapRender::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
    glfwSwapBuffers(window);
}

void MapRender::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,true);
}

void MapRender::renderStaticMap(const GridMap &static_map)
{
    glClearColor(1.f,1.f,1.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.f,1.f,0.f);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(-0.5f,0.f);
    glVertex2f(0.5f,0.5f);
    glEnd();

    glColor3f(1.0f,0.0f,0.0f);
    glPointSize(20);
    glBegin(GL_POINTS);
    for(size_t i = 0; i < static_map._width; ++i)
    {
        for(size_t j = 0; j < static_map._height; ++j)
        {
            glVertex2i(i , j);
        }
    }
    glEnd();
    glFinish();
}

