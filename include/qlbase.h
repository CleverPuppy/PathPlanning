#ifndef QLBASE_H
#define QLBASE_H

#include <vector>
/*
 * QLearning base class
 * Contains basic qlearning
*/
class QLBase
{
public:
    /*
     * qlearning parameters
    */
    float learning_rate;
    float gamma;
    float epsilon;
    /*
     * qlearning data
     */
    std::vector<std::vector<float>> qtable;
    QLBase(size_t state_dim,
           size_t action_dim,
           float learning_rate,
           float gamma,
           float epsilon);
    ~QLBase() = default;
    void update(size_t state,
                size_t action,
                float reward,
                size_t next_state,
                float& loss);
};

#endif // QLBASE_H
