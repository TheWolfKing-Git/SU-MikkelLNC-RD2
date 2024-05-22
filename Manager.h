#ifndef MANAGER_H
#define MANAGER_H
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <iostream>

#include "Enemy.h"
#include "Hero.h"


class Manager
{
public:
    Manager(Hero hero, Enemy enemy, QSqlDatabase gameDatabase);

    //Enemy handling
    void setEnemy(Enemy newEnemy);
    void addEnemies();
    Enemy loadEnemy(int enemyID);
    void printEnemies();
    void printEnemyStats();


    //Hero handling
    void setHero(Hero newHero);
    void saveHero();
    Hero loadHero(int heroID);
    void printHeros();
    void printHeroStats();

    //Fighting
    int Encounter();
    void nextPhase();


private:
    Hero mHero;
    Enemy mEnemy;
    QSqlQuery mGameQuery;
    QSqlDatabase mDB;

};

#endif // MANAGER_H
