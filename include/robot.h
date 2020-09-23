#ifndef ROBOT_H
#define ROBOT_H

#include "graph.h"
#include <unordered_map>
#include <list>
#include <iostream>
using UUID = uint64_t;
enum class GroundAGV_ACTION
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    IDLE
};

enum class RobotStatus
{
    ON,
    OFF,
    OFFLINE,
    UPDATED,
    SUSPEND
};

struct Robot
{
    RobotStatus _status;
    UUID _uuid;

    Robot(UUID uuid) : _uuid(uuid), _status(RobotStatus::ON){}
    ~Robot() = default;
};

struct GroundAGV : public Robot
{
    vec2d _current_location;
    vec2d _target_location;
    GroundAGV_ACTION action;

    GroundAGV(UUID uuid, vec2d _start, vec2d _dest)
        : Robot(uuid), _current_location(_start), _target_location(_dest),
          action(GroundAGV_ACTION::IDLE){}
    ~GroundAGV() = default;
};
std::ostream& operator <<(std::ostream& os, const GroundAGV& agv);

class MultiAGVManager
{
public:
    size_t _size;
    GridMap static_map;
    std::list<GroundAGV> _robots;
    std::unordered_map<UUID, GroundAGV*> _robots_map;

    MultiAGVManager(GridMap&& map) : static_map(std::forward<GridMap>(map)), _size(0) {}
    ~MultiAGVManager() = default;

    void addRobot(UUID uuid, vec2d start, vec2d dest);
    void takeSnap(std::ostream& os);
};

#endif // ROBOT_H
