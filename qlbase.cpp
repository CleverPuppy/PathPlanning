#include "qlbase.h"
#include <algorithm>
#include <cmath>

QLBase::QLBase(size_t state_dim, size_t action_dim, float learning_rate, float gamma, float epsilon)
    :learning_rate(learning_rate),
      gamma(gamma),
      epsilon(epsilon),
      qtable(state_dim,std::vector<float>(action_dim,0.0f))
{

}

void QLBase::update(size_t state, size_t action, float reward, size_t next_state, float& loss)
{
    float prev = qtable[state][action];
    qtable[state][action] = (1 - learning_rate) * qtable[state][action] +  learning_rate * (
                reward + gamma * *std::max_element(qtable[next_state].begin(), qtable[next_state].end()));
    loss = fabs(qtable[state][action] - prev);
}
