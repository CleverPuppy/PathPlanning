#include "pathplan.h"
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <list>

struct AStarNode{
    size_t x;
    size_t y;
    size_t gscore;
    size_t dist;        // 到起点的距离
    const AStarNode* prev;
    AStarNode(size_t _x, size_t _y,size_t _dist, size_t _score, const AStarNode* node)
        : x(_x), y(_y), dist(_dist), gscore(_score), prev(node) {}
};

std::vector<unsigned int> AStar(const GridMap &map, GroundAGV &rb, MapRender* render)
{
    auto goal_x = rb._target_location.x;
    auto goal_y = rb._target_location.y;
    auto cmp = [](const AStarNode* p1, const AStarNode* p2){
        return p1->gscore > p2->gscore;
    };
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, decltype(cmp)> openset(cmp);
    std::list<AStarNode> searchedNode {AStarNode{rb._current_location.x,
                                                  rb._current_location.y,
                                                  0,
                                                  0,
                                                  nullptr}};
    openset.push(&searchedNode.front());
    std::unordered_set<size_t> closed;
    auto addNeibor = [&](size_t x, size_t y, const AStarNode* parent){
        if(x >= 0 && x < map._width && y >= 0 && y < map._height
                && closed.count(y * map._width + x) == 0
                && map._map[x][y] == '.'){
            if(render!= nullptr){
                render->drwaRect(x,y);
                render->updateWindow();
            }
            auto gscore = parent->dist + 1
                            +  (x > goal_x ? x - goal_x : goal_x - x)
                            +  (y > goal_y ? y - goal_y : goal_y - y);
            searchedNode.emplace_back(AStarNode{x,y,parent->dist + 1,gscore, parent});
            openset.push(&searchedNode.back());
            closed.insert(y * map._width + x);
        }
    };
    while(!openset.empty())
    {
        const AStarNode* node = openset.top();
        if(node->x == goal_x && node->y == goal_y){
            std::cout << "reach destination" << std::endl;
            std::vector<unsigned int> ans;
            while(node != nullptr){
                ans.emplace_back(node->y  * map._width + node->x);
                node = node->prev;
            }
            std::reverse(ans.begin(), ans.end());
            return ans;
        }

        openset.pop();

        /*
         * check neibor
        */
        size_t ux,uy,dx,dy,lx,ly,rx,ry;
        ux = dx = node->x;
        ry = ly = node->y;
        uy = ry - 1;
        dy = ry + 1;
        lx = ux - 1;
        rx = ux + 1;
        addNeibor(ux,uy,node);
        addNeibor(dx,dy,node);
        addNeibor(lx,ly,node);
        addNeibor(rx,ry,node);
    }
    return {};
}

SRPL_QL::SRPL_QL(size_t map_width, size_t map_height, float learning_rate, float gamma, float epsilon)
    : QLBase(map_width * map_height, 4, learning_rate, gamma, epsilon),
      staticMap(map_width, map_height)
{

}

void SRPL_QL::run(unsigned int episode_times)
{
    auto& static_map = staticMap._map;
    auto width = staticMap._width;
    auto height = staticMap._height;
    for(auto i = 0UL; i < width; ++i)
    {
        for(auto j = 0UL; j < height; ++j)
        {
            // TODO SELECT ALL POSITIONS
        }
    }
    for(unsigned int i = 0; i < episode_times; ++i)
    {
        /*
         * random select a start location and a destination location
        */

    }

}
