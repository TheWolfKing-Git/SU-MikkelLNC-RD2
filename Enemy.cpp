#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::Enemy(std::string newName)
{
    mName = newName;
}


//Getters
int Enemy::getXPReward() const{
    return mXPReward;
}
std::string Enemy::getName() const{
    return mName;
}

//Setters
void Enemy::setXPReward(int newXPReward){
    mXPReward = newXPReward;
}

