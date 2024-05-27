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
std::string Enemy::getElement() const{
    return mElement;
}

//Setters
void Enemy::setXPReward(int newXPReward){
    mXPReward = newXPReward;
}
void Enemy::setElement(std::string newElement){
    mElement = newElement;
}


