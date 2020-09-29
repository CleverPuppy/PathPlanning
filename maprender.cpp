#include "maprender.h"
#include "errno.h"
#include <cstring>
#include "unistd.h"
#include <thread>


MapRender::MapRender()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,800.0,0.0,600.0);
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
    glfwPollEvents();
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

void MapRender::render(const GridMap &static_map)
{
    glClear(GL_COLOR_BUFFER_BIT);
    /*
     * render points
    */
    glColor3f(1.0f,0.0f,0.0f);
    glPointSize(4);
    glBegin(GL_POINTS);
    for(size_t i = 0; i < static_map._width; ++i)
    {
        for(size_t j = 0; j < static_map._height; ++j)
        {
            glVertex2i(gap + i * gap ,gap + j * gap);
        }
    }
    glEnd();

    /*
     * render obstacles
    */
    glColor3f(0.0f,0.0f,0.0f);
    glPolygonMode(GL_POINT,GL_FILL);
    for(size_t i = 0; i < static_map._width; ++i)
    {
        for(size_t j = 0; j < static_map._height; ++j)
        {
            if(static_map._map[i][j] == '#'){
                int cx = (i + 1) * gap;
                int cy = (j + 1) * gap;
                glBegin(GL_POLYGON);
                glVertex2f(cx - gap / 2.f, cy - gap / 2.f);
                glVertex2f(cx - gap / 2.f, cy + gap / 2.f);
                glVertex2f(cx + gap / 2.f, cy + gap / 2.f);
                glVertex2f(cx + gap / 2.f, cy - gap / 2.f);
                glEnd();
            }
        }
    }

    /*
     * render stock
    */
    glColor3f(0.0f,1.0f,0.0f);
    glPolygonMode(GL_POINT,GL_FILL);
    for(size_t i = 0; i < static_map._width; ++i)
    {
        for(size_t j = 0; j < static_map._height; ++j)
        {
            if(static_map._map[i][j] == 'S'){
                int cx = (i + 1) * gap;
                int cy = (j + 1) * gap;
                glBegin(GL_POLYGON);
                glVertex2f(cx - gap / 3.f, cy - gap / 3.f);
                glVertex2f(cx - gap / 3.f, cy + gap / 3.f);
                glVertex2f(cx + gap / 3.f, cy + gap / 3.f);
                glVertex2f(cx + gap / 3.f, cy - gap / 3.f);
                glEnd();
            }
        }
    }
}

void MapRender::render(const MultiAGVManager &manager)
{
    /*
     * clear window
    */
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    /*
     * render static map
    */
    render(manager.static_map);
    /*
     * render all robots
    */
    glColor3f(0.0f,0.0f,1.0f);
    glPolygonMode(GL_POINT, GL_FILL);

    for(const auto& robot : manager._robots)
    {
        render(robot);
    }

    glFinish();
}

void MapRender::drwaRect(unsigned int x, unsigned int y)
{
    glColor4f(0.f,0.f,0.f,0.4f);
    auto cx = gap + x * gap;
    auto cy = gap + y * gap;
    glBegin(GL_POLYGON);
    glVertex2f(cx - gap / 3.f, cy - gap / 3.f);
    glVertex2f(cx - gap / 3.f, cy + gap / 3.f);
    glVertex2f(cx + gap / 3.f, cy + gap / 3.f);
    glVertex2f(cx + gap / 3.f, cy - gap / 3.f);
    glEnd();
}

inline void MapRender::render(const GroundAGV &rb)
{
    int cx = gap + rb._current_location.x * gap;
    int cy = gap + rb._current_location.y * gap;
    glBegin(GL_POLYGON);
    glVertex2f(cx - gap / 3.f, cy - gap / 3.f);
    glVertex2f(cx - gap / 3.f, cy + gap / 3.f);
    glVertex2f(cx + gap / 3.f, cy + gap / 3.f);
    glVertex2f(cx + gap / 3.f, cy - gap / 3.f);
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    glVertex2i(cx,cy);
    glVertex2i(gap + rb._target_location.x * gap, gap + rb._target_location.y * gap);
    glEnd();
}
