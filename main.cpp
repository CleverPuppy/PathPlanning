#include <iostream>
#include "graph.h"
#include "robot.h"
using namespace std;

int main()
{
    GridMap&& map{};
    MultiAGVManager manager{std::move(map)};
    manager.addRobot(1, {1,1}, {10,10});
    return 0;
}
