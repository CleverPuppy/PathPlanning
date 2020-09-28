#ifndef PATHPLAN_H
#define PATHPLAN_H
#include "graph.h"
#include "robot.h"
#include "maprender.h"
#include "qlbase.h"
#include <vector>

/*
 * single robot offline path planning
*/
std::vector<unsigned int> AStar(const GridMap& map, GroundAGV& rb, MapRender* render = nullptr);

/*
 * Single-Robot-Path-Plan-QLearning
 *
 * State - Robot Location and relative Goal
 * Action - UP DOWN LEFT RIGHT
 * Reward - Delta Manhattan Distance,
 *          Reach destination get a Maxium reward and end Episode
 *          Reach Obstacle get a negative reward and end Episode
*/
class SRPL_QL : public QLBase
{
private:
    GridMap staticMap;

    size_t getRelativeDirection(size_t x, size_t y, size_t goal_x, size_t goal_y);
public:
    SRPL_QL(size_t map_width, size_t map_height,
            float learning_rate,
            float gamma,
            float epsilon);
    /*
     * run  Episodes
    */
    void run(unsigned int episode_times);
};
#endif // PATHPLAN_H
