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
    _height = _map.size();
    _width = _map[0].size();
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

    /*
     * check start point and destination
    */
    size_t count_s = 0;
    size_t count_d = 0;
    for(size_t _row = 0; _row < _height; ++_row)
    {
        for(size_t _col = 0; _col < _width; ++_col)
        {
            if(_map[_row][_col] == 'S'){
                count_s ++;
                _start.x = _col;
                _start.y = _row;
            }else if(_map[_row][_col] == 'D')
            {
                count_d ++;
                _destination.x = _col;
                _start.y = _row;
            }
        }
    }
    if(count_s != 1 || count_d != 1){
        std::cerr << "GridMap::mapValidCheck failed!" << std::endl
                  << "'S' denotes start : " << count_s << " should only have one" << std::endl
                  << "'D' denotes destination : " << count_d << " should only have one" << std::endl;
        return false;
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
                _map[row][col] = '#';
                col++;
            }
        }
    }

    /*
     * generate Start and Destination
    */
    std::random_device rd;
    std::default_random_engine rd_engine(rd());
    std::uniform_int_distribution<size_t> col_dist(1, _width - 2);
    std::uniform_int_distribution<size_t> row_dist(1, _height - 2);
    auto generateCol = std::bind(col_dist,rd_engine);
    auto generateRow = std::bind(row_dist,rd_engine);
    auto s_row = generateRow();
    auto d_row = generateRow();
    auto s_col = generateCol();
    auto d_col = generateCol();
    while(_map[s_row][s_col] != '.'){
        s_row = generateRow();
        s_col = generateCol();
    }
    _map[s_row][s_col] = 'S';
    _start.x = s_col;
    _start.y = s_row;
    while(_map[d_row][d_col] != '.'){
        d_row = generateRow();
        d_col = generateCol();
    }
    _map[d_row][d_col] = 'D';
    _destination.x = d_col;
    _destination.y = d_row;
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
