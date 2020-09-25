#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>

struct vec2d
{
    size_t x;
    size_t y;
    vec2d(size_t _x = 0, size_t _y = 0): x(_x), y(_y) {}
    ~vec2d() = default;
};
std::ostream& operator <<(std::ostream& os, const vec2d& v2d);
/*
 * struct GridMap
 * ! Make sure map surrounded by obstacles
 * ! Make sure map have and only have one startPlace and destination
 * [.]  freespace
 * [#]  obstacle
 * [R]  robot
 * [D]  destination
 * [S]  Stock
*/
struct GridMap
{
    std::vector<std::vector<char>> _map;
    const size_t _width;                              //地图宽度
    const size_t _height;                             //地图长度

    /*
     * width and height mush larger than 3
    */
    GridMap(unsigned int width = 20, unsigned int height = 20);
    ~GridMap() = default;
    bool mapValidCheck();                       //检查地图是否符合标准
    void randomGenerateMap();                   //随机生成地图

    size_t toID(size_t row , size_t col) const;
    std::pair<size_t, size_t> toXY(size_t id) const;
};
std::ostream& operator<<(std::ostream& os, const GridMap& grid_map);

#endif // GRAPH_H
