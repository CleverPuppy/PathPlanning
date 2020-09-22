#include "robot.h"

void MultiAGVManager::addRobot(UUID uuid, vec2d start, vec2d dest){
    _robots.emplace_front(GroundAGV(uuid, start, dest));
    _robots_map.insert({uuid, &_robots.back()});
    _size++;
}

void MultiAGVManager::takeSnap(std::ostream &os)
{
    /*
     * Take a snap
     * snap contains mamy components
     * 1. a static 2d grid map
     * 2. all robots info
    */

    os << this->static_map << std::endl;
    if(_robots.empty()){
        os << "None Robots" << std::endl;
    }
    for(const auto& robot : _robots)
    {
        os << robot << std::endl;
    }

}

std::ostream &operator <<(std::ostream &os, const GroundAGV &agv)
{
    os << agv._uuid << " "
       << agv._current_location << " "
       << agv._target_location << " "
       << (int)agv._status << " "
       << (int)agv.action << std::endl;
    return os;
}
