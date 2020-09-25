#include "graph.h"

#include <iostream>
#include <random>
#include <ctime>
#include <functional>

GridMap::GridMap(unsigned int width, unsigned int height)
    :_width(width), _height(height)
{
    randomGenerateMap();
}

bool GridMap::mapValidCheck()
{
    if(_map.size() < 2 || _map[0].size() < 2) return false;

    /*
     * check obstacles
    */
    for(size_t _row : {0UL, _height - 1})
    {
        for(size_t _col = 0; _col < _width; ++_col)
        {
            if(_map[_row][_col] != '#'){
                std::cerr << "GridMap::mapValidCheck failed!"
                          << "Position " << _row << "-"<<_col << " should be '#'"
                          << std::endl;
                return false;
            }
        }
    }
    for(size_t _col : {0UL, _width - 1})
    {
        for(size_t _row = 0; _row < _height; ++_row)
        {
            if(_map[_row][_col] != '#'){
                std::cerr << "GridMap::mapValidCheck failed!"
                          << "Position " << _row << "-"<<_col << " should be '#'"
                          << std::endl;
                return false;
            }
        }
    }

    return true;
}

void GridMap::randomGenerateMap()
{
    _map.resize(_height, std::vector<char>(_width, '.'));
    /*
     * generate boarder
    */
    for(size_t _row : {0UL, _height - 1})
    {
        for(size_t _col = 0; _col < _width; ++_col)
        {
            _map[_row][_col] = '#';
        }
    }
    for(size_t _col : {0UL, _width - 1})
    {
        for(size_t _row = 0; _row < _height; ++_row)
        {
            _map[_row][_col] = '#';
        }
    }

    /*
     * generate Obstacles
    */
    static const int longestObs = 10;
    static const int gapSize = 1;

    for(size_t row = 1 + gapSize; row < _height - 1 - gapSize; row += gapSize + 1)
    {
        for(size_t col = 1 + gapSize; col < _width - 1; col += gapSize)
        {
            int count = 0;
            while(count++ < longestObs && col < _width - 1 - gapSize)
            {
                _map[row][col] = 'S';
                col++;
            }
        }
    }

}

inline size_t GridMap::toID(size_t row, size_t col) const
{
    return row * _width + col;
}

inline std::pair<size_t, size_t> GridMap::toXY(size_t id) const
{
    return {id / _width, id % _width};
}

std::ostream &operator<<(std::ostream &os, const GridMap &grid_map)
{
    os << "Map [" << grid_map._height << "," << grid_map._width << "]" << std::endl;
    for(const auto& row : grid_map._map)
    {
        for(char c : row)
        {
            os << c << ' ';
        }
        os << std::endl;
    }
    return os;
}

std::ostream &operator <<(std::ostream &os, const vec2d &v2d)
{
    os << "[" << v2d.x << ","<< v2d.y << "]";
}
