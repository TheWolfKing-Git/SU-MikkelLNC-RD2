#include "Magic.h"

Magic::Magic()
{
    mID = 0;
    mName = "";
    mDMG = 0;
    mSelfDMG = 0;
    mElement = "";
    mStrengt = "";
    mWeakness = "";
    mManaCost = 0;
    mGoldCost = 0;
}

// Getters
int Magic::getID() const
{
    return mID;
}

std::string Magic::getName() const
{
    return mName;
}

int Magic::getDMG() const
{
    return mDMG;
}

int Magic::getSelfDMG() const
{
    return mSelfDMG;
}

std::string Magic::getElement() const
{
    return mElement;
}

std::string Magic::getStrength() const
{
    return mStrengt;
}

std::string Magic::getWeakness() const
{
    return mWeakness;
}

int Magic::getManaCost() const
{
    return mManaCost;
}

int Magic::getGoldCost() const
{
    return mGoldCost;
}

// Setters
void Magic::setID(int newID)
{
    mID = newID;
}

void Magic::setName(std::string newName)
{
    mName = newName;
}

void Magic::setDMG(int newDMG)
{
    mDMG = newDMG;
}

void Magic::setSelfDMG(int newSelfDMG)
{
    mSelfDMG = newSelfDMG;
}

void Magic::setElement(std::string newElement)
{
    mElement = newElement;
}

void Magic::setStrength(std::string newStrengt)
{
    mStrengt = newStrengt;
}

void Magic::setWeakness(std::string newWeakness)
{
    mWeakness = newWeakness;
}

void Magic::setManaCost(int newManaCost)
{
    mManaCost = newManaCost;
}

void Magic::setGoldCost(int newGoldCost)
{
    mGoldCost = newGoldCost;
}
