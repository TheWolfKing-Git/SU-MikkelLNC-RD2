#ifndef MANAGER_H
#define MANAGER_H
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <iostream>

#include "Enemy.h"
#include "Hero.h"


class Manager
{
public:
    Manager(Hero hero, Enemy enemy, QSqlDatabase gameDatabase);

    //Enemy handling
    void addEnemies();
    void printEnemies();

    //Hero handling
    void saveHero();
    void loadHero(int heroID);
    void printHeros();


private:
    Hero mHero;
    Enemy mEnemy;
    QSqlQuery mGameQuery;
    QSqlDatabase mDB;

};

#endif // MANAGER_H
