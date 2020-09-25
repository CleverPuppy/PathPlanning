#ifndef PATHPLAN_H
#define PATHPLAN_H
#include "graph.h"
#include "robot.h"
#include "maprender.h"
#include <vector>

std::vector<unsigned int> AStar(const GridMap& map, GroundAGV& rb, MapRender* render = nullptr);
#endif // PATHPLAN_H
