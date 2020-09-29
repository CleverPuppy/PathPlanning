#include "pathplan.h"
#include <unordered_set>
#include <queue>
#include <cmath>
#include <algorithm>
#include <list>
#include <random>
#include <functional>
#include <unistd.h>

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

inline size_t SRPL_QL::getRelativeDirection(size_t x, size_t y, size_t goal_x, size_t goal_y)
{
    if(x == goal_x){
        if(y < goal_y){
            return 0;
        }
        if(y > goal_y){
            return 1;
        }
    }
    if(y == goal_y){
        if(x < goal_x){
            return 2;
        }
        if(x > goal_x){
            return 3;
        }
    }
    if(x < goal_x){
        if(y < goal_y){
            return 4;
        }
        if(y > goal_y){
            return 5;
        }
    }else{
        if(y < goal_y){
            return 6;
        }
        if(y > goal_y){
            return 7;
        }
    }
    return 8;
}

SRPL_QL::SRPL_QL(size_t map_width, size_t map_height, float learning_rate, float gamma, float epsilon)
    : QLBase(map_width * map_height * 9, 4, learning_rate, gamma, epsilon),
      staticMap(map_width, map_height)
{

}

void SRPL_QL::run(unsigned int episode_times, MapRender* render)
{
    auto& static_map = staticMap._map;
    auto width = staticMap._width;
    auto height = staticMap._height;
    auto state_size = qtable.size();
    auto action_size = qtable.front().size();
    std::vector<size_t> vCandidates;        // 候选集
    for(auto i = 0UL; i < width; ++i)
    {
        for(auto j = 0UL; j < height; ++j)
        {
            // 假设机器人都从货架左边取货
            if(static_map[i][j] == 'S'){
                vCandidates.emplace_back(j * width + i - 1);
            }
        }
    }
    /*
     * init random module
    */
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> rd_distribution(0,vCandidates.size() - 1);
    std::uniform_real_distribution<float> epsilon_prob(0.0f,1.0f);
    std::uniform_int_distribution<size_t> action_dist(0, action_size - 1);
    auto dice = std::bind(rd_distribution, engine);
    auto robot_location = vCandidates[dice()];
    size_t destination = vCandidates[dice()];
    size_t state;
    size_t nextState;
    for(unsigned int i = 0; i < episode_times; ++i)
    {
        /*
         * random select a destination location
        */
//        destination = vCandidates[dice()];
        size_t goal_x = destination % width;
        size_t goal_y = destination / width;
        size_t x = robot_location % width;
        size_t y = robot_location / width;
        bool episodeShouldEnd = false;
        float average_loss = 0.0f;
        size_t count = 0;
        while(!episodeShouldEnd)   // !!! 确保候选集中的节点之间能够互相连通
        {
            count ++;
            /*
             * storage current state info
            */
            auto x_backup = x;
            auto y_backup = y;
            size_t manh_distance = (x > goal_x ? x - goal_x : goal_x - x) + (y > goal_y ? y - goal_y : goal_y - y);
            state = (y * width + x) * 9 + getRelativeDirection(x,y,goal_x,goal_y);
            /*
             * select action. using epsilon greedy
            */
            size_t best_action = 0;
            float best_reward = -__FLT_MAX__;
            // using epsilon greedy
            if(epsilon_prob(engine) < epsilon){
                for(auto action = 0; action < action_size; ++action)
                {
                    if(qtable[state][action] > best_reward){
                        best_action = action;
                        best_reward = qtable[state][action];
                    }
                }
            }else{
                best_action = action_dist(engine);
            }

            if(render != nullptr){
                GroundAGV agv(0,{x,y},{goal_x,goal_y});
                render->render(staticMap);
                render->render(agv);
                render->updateWindow();
                sleep(0.1);
            }

            /*
             * determine next state or if epsode finished{ reach obstacle or destination}
            */
            switch (best_action) {
            case 0: // UP
                y -= 1;
                break;
            case 1: // DOWN
                y += 1;
                break;
            case 2: // LEFT
                x -= 1;
                break;
            case 3: // RIGHT;
                x += 1;
                break;
            }
            float reward = -1.f;
            if(x == goal_x && y == goal_y){
                reward = 10.f;
                episodeShouldEnd = true;
                nextState = (y * width + x) * 9 + getRelativeDirection(x,y,goal_x,goal_y);
            }else if(static_map[x][y] == '.'){
                reward = (long)manh_distance - long((x > goal_x ? x - goal_x : goal_x - x) + (y > goal_y ? y - goal_y : goal_y - y));
                nextState = (y * width + x) * 9 + getRelativeDirection(x,y,goal_x,goal_y);
            }else{
                reward = -5.f;
                nextState = state;
                x = x_backup;
                y = y_backup;
            }
            /*
             * update state
            */
            float _loss;
            update(state, best_action,reward,nextState, _loss);
            average_loss += _loss;
            state = nextState;
        }
        average_loss /= count;
        std::cout << average_loss << std::endl;
    }

}
