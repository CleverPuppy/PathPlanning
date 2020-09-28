#include <iostream>
#include <cstdio>
#include <cstring>
#include "graph.h"
#include "robot.h"
#include "maprender.h"
#include "pathplan.h"
#include "GL/gl.h"
#include "GLFW/glfw3.h"
#include "unistd.h"
using namespace std;

int main()
{
    GridMap&& map{};
    MultiAGVManager manager{std::move(map)};

    MapRender render{};
    render.render(manager);
    render.updateWindow();
    manager.addRobot(1, {1,1}, {9,10});
    manager.addRobot(2, {1,10}, {9,11});

    auto ans =AStar(manager.static_map,manager._robots.front(),&render);
    for(unsigned int id : ans)
    {
        size_t y = id / manager.static_map._width;
        size_t x = id % manager.static_map._width;
        manager._robots.front()._current_location = {x,y};
        render.render(manager);
        render.updateWindow();
        sleep(1);
    }

    auto ans2 = AStar(manager.static_map, manager._robots.back(), &render);
    for(unsigned int id : ans2)
    {
        size_t y = id / manager.static_map._width;
        size_t x = id % manager.static_map._width;
        manager._robots.back()._current_location = {x,y};
        render.render(manager);
        render.updateWindow();
        sleep(1);
    }

    SRPL_QL singleRobotQL(20,20,0.2,0.9,0.9);
    singleRobotQL.run(100);
    return 0;
}
