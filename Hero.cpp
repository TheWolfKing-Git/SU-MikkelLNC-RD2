#include "Hero.h"

Hero::Hero()
{

}

//Getters
int Hero::getLevel() const{
    return mLevel;
}
int Hero::getCurrentXP() const{
    return mCurrentXP;
}

//Setters
void Hero::setLevel(int newLevel){
    mLevel = newLevel;
}
void Hero::setXP(int newXP){
    mCurrentXP = newXP;
}

//Functions
void Hero::addXP(int addXP){

    mCurrentXP += addXP;
    if(mCurrentXP >= mLevel * 1000){
        mCurrentXP -= mLevel * 1000;
        ++mLevel;
    }
}
