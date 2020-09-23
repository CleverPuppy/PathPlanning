#include <iostream>
#include <cstdio>
#include <cstring>
#include "graph.h"
#include "robot.h"
#include "maprender.h"
#include "GL/gl.h"
#include "GLFW/glfw3.h"
using namespace std;

int main()
{
    GridMap&& map{};
    MultiAGVManager manager{std::move(map)};

    MapRender render{};
    render.renderStaticMap(manager.static_map);
    render.updateWindow();
    manager.addRobot(1, {1,1}, {10,10});
    return 0;
}
