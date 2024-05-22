#include "Character.h"

//Default
Character::Character(){
    mName = "";
}

//Getters
int Character::getHP() const{
    return mHP;
}
int Character::getDMG() const{
    return mDMG;
}

//Setters
void Character::setHP(int newHP){
    mHP = newHP;
}
void Character::setDMG(int newDMG){
    mDMG = newDMG;
}

//Alive?
bool Character::isAlive() const{
    return mHP > 0;
}

//Take DMG
void Character::takeDMG(int incDMG){
    if(incDMG > 0)
        mHP -= incDMG;
    else if(incDMG < 0)
        mHP += incDMG;
}
