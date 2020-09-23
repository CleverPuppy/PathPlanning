#include "maprender.h"
#include "errno.h"
#include <cstring>
#include "unistd.h"

MapRender::MapRender()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glViewport(0,0,800,600);
    glfwSetFramebufferSizeCallback(window,&MapRender::framebuffer_size_callback);
}

MapRender::~MapRender()
{
    glfwTerminate();
}

void MapRender::render()
{
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void MapRender::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
}
