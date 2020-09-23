#include <iostream>
#include "graph.h"
#include "robot.h"
#include "maprender.h"

using namespace std;

int main()
{
    GridMap&& map{};
    MultiAGVManager manager{std::move(map)};

    MapRender render{};
    render.render();
    manager.addRobot(1, {1,1}, {10,10});
    return 0;
}
