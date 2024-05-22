#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <iostream>

#include "Enemy.h"
#include "Hero.h"

int main()
{
    int GameState = 0;

    Hero gameHero;
    std::string heroName;

    //Setup database
    QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL");
    DB.setHostName("localhost"); // Currently set to localhost
    DB.setUserName("sammy"); // Change to username
    DB.setPassword("password"); // Change to password

    //Check open connection
    if (!DB.open()) {
        qDebug() << "Failed to connect to database:";
        qDebug() << DB.lastError().text();
        return 1; // Exit with error code
    }

    //Make QSQL object to DB
    QSqlQuery gameQuery(DB);

    //Create main database for game
    QString createDB = "CREATE DATABASE IF NOT EXISTS Game";
    if (!gameQuery.exec(createDB)) {
        qDebug() << "Failed to create database:";
        qDebug() << gameQuery.lastError().text();
        return 1; // Exit with error code
    }

    //Change the current database to "Game"
    DB.setDatabaseName("Game");
    if (!DB.open()) {
        qDebug() << "Failed to select database:";
        qDebug() << DB.lastError().text();
        return 1; // Exit with error code
    }

    //Create table for Hero
    QString createTB1 = "CREATE TABLE IF NOT EXISTS Hero ("
            "ID INT AUTO_INCREMENT PRIMARY KEY,"
            "Name VARCHAR(255) UNIQUE,"
            "HP INT,"
            "DMG INT,"
            "Level INT,"
            "XP INT"
            ")";
    if (!gameQuery.exec(createTB1)) {
        qDebug() << "Failed to create Hero table:";
        qDebug() << gameQuery.lastError().text();
        return 1; // Exit with error code
    }

    //Create table for Enemy
    QString createTB2 = "CREATE TABLE IF NOT EXISTS Enemy ("
            "ID INT AUTO_INCREMENT PRIMARY KEY,"
            "Name VARCHAR(255) UNIQUE,"
            "HP INT,"
            "DMG INT,"
            "XPReward INT"
            ")";
    if (!gameQuery.exec(createTB2)) {
        qDebug() << "Failed to create Enemy table:";
        qDebug() << gameQuery.lastError().text();
        return 1; // Exit with error code
    }

    //After DB creation, insert enemies
    QList<QVariantList> enemies = {
        {"Weak", 2, 1, 100},
        {"Medium", 5, 2, 300},
        {"Strong", 10, 5, 500}
    };


    for (const auto& enemy : enemies) {
        gameQuery.prepare("INSERT INTO Enemy (Name, HP, DMG, XPReward) VALUES (?, ?, ?, ?)");
        gameQuery.addBindValue(enemy[0]); // Name
        gameQuery.addBindValue(enemy[1]); // HP
        gameQuery.addBindValue(enemy[2]); // DMG
        gameQuery.addBindValue(enemy[3]); // XPReward

        //Execute the query
        if (!gameQuery.exec()) {

            // Check if the error is due to a duplicate entry
            if (gameQuery.lastError().number() == 1062) // MySQL error code for duplicate entry
            {
                qDebug() << "Skipping duplicate entry for enemy:" << enemy[0].toString();
                gameQuery.clear(); // Clear the query object for the next iteration
                continue; // Skip to the next iteration
            }
            else
            {
                qDebug() << "Failed to insert enemy:";
                qDebug() << gameQuery.lastError().text();
                return 1; // Exit with error code
            }
        }
        gameQuery.clear(); // Clear the query object for the next iteration
    }

    while (true) {
        std::cout << "Select an option:" << std::endl;
        std::cout << "1: Start adventure with a new Hero! " << std::endl;
        std::cout << "2: Continue an adventure with a Hero!" << std::endl;
        std::cout << "3: Show Heros ready for adventure!" << std::endl;
        std::cout << "4: Show Enemies for slaying!" << std::endl;
        std::cout << "11: Exit" << std::endl;
        std::cin >> GameState;

        switch (GameState) {
            case 1:
                std::cout << "Starting new adventure!" << std::endl;
                std::cout << "Name your Hero: " << std::endl;
                std::cin >> heroName;
                gameHero = Hero(heroName);
                break;
            case 2:
                std::cout << "You selected Option 2" << std::endl;
                // Code for Option 2
                break;
            case 3:
                std::cout << "You selected Option 3" << std::endl;
                // Code for Option 3
                break;
            case 4:
                std::cout << "You selected Option 3" << std::endl;
                // Code for Option 3
                break;
            case 11:
                std::cout << "Exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }

    return 1;
}
