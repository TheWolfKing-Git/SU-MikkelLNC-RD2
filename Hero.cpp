#include "Hero.h"

Hero::Hero()
{
    mName = "Placeholder";
    mHP = 10;
    mDMG = 1;
    mLevel = 1;
    mCurrentXP = 0;
    mGold = 0;
}

Hero::Hero(std::string newName)
{
    mName = newName;
    mHP = 10;
    mDMG = 1;
    mLevel = 1;
    mCurrentXP = 0;
    mGold = 0;
}

//Getters
int Hero::getLevel() const{
    return mLevel;
}
int Hero::getCurrentXP() const{
    return mCurrentXP;
}
std::string Hero::getName() const{
    return mName;
}
int Hero::getGold() const{
    return mGold;
}
int Hero::getMana() const{
    return mMana;
}

//Setters
void Hero::setLevel(int newLevel){
    mLevel = newLevel;
}
void Hero::setXP(int newXP){
    mCurrentXP = newXP;
}
void Hero::setName(std::string newName){
    mName = newName;
}
void Hero::setGold(int newGold){
    mGold = newGold;
}
void Hero::setMana(int newMana){
    mMana = newMana;
}

//Functions
void Hero::addXP(int addedXP){
    mCurrentXP += addedXP;
    if(mCurrentXP > mLevel*1000){
        mCurrentXP -= (mLevel * 1000);
        addLevel(1);
        std::cout << "Your exprience shows and you gained a level!" << std::endl;
        std::cout <<"Level increased from: " << getLevel()-1 << " to " << getLevel() << std::endl;
        std::cout <<"HP increased to: " << getHP() << std::endl;
        std::cout <<"DMG increased to: " << getDMG() << std::endl;
    }
}

void Hero::addLevel(int addedLevel){
    mLevel += addedLevel;
    mDMG += 1;
    mHP += 2;
    mMana += 2;
}

void Hero::resetHero(){
    mHP = 10+(mLevel*2-2);
}

void Hero::addGold(int addedGold){
    mGold += addedGold;
}

void Hero::addMana(int addedMana){
    mMana += addedMana;
}
