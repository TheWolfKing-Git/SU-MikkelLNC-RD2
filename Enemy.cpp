#include "Enemy.h"

Enemy::Enemy()
{

}

//Getters
int Enemy::getXPReward() const{
    return mXPReward;
}

//Setters
void Enemy::setXPReward(int newXPReward){
    mXPReward = newXPReward;
}

