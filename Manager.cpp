#include "Manager.h"

Manager::Manager(Hero hero, Enemy enemy, QSqlDatabase gameDatabase)
{

        mHero = hero;
        mEnemy = enemy;
        mDB = gameDatabase;

        //Check open connection
        if (!mDB.open()) {
            qDebug() << "Failed to connect to database:";
            qDebug() << mDB.lastError().text();
        }

        //Create main database for game
        QString createDB = "CREATE DATABASE IF NOT EXISTS Game";
        if (!mGameQuery.exec(createDB)) {
            qDebug() << "Failed to create database:";
            qDebug() << mGameQuery.lastError().text();

        }

        //Change the current database to "Game"
        mDB.setDatabaseName("Game");
        if (!mDB.open()) {
            qDebug() << "Failed to select database:";
            qDebug() << mDB.lastError().text();
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
        if (!mGameQuery.exec(createTB1)) {
            qDebug() << "Failed to create Hero table:";
            qDebug() << mGameQuery.lastError().text();
        }

        //Create table for Enemy
        QString createTB2 = "CREATE TABLE IF NOT EXISTS Enemy ("
                "ID INT AUTO_INCREMENT PRIMARY KEY,"
                "Name VARCHAR(255) UNIQUE,"
                "HP INT,"
                "DMG INT,"
                "XPReward INT"
                ")";
        if (!mGameQuery.exec(createTB2)) {
            qDebug() << "Failed to create Enemy table:";
            qDebug() << mGameQuery.lastError().text();
        }

}
//------------------------------------------------ Enemy Handling --------------------------------------------------------
void Manager::addEnemies()
{
    //After DB creation, insert enemies
    QList<QVariantList> enemies = {
        {"Weak", 2, 1, 100},
        {"Medium", 5, 2, 300},
        {"Strong", 10, 5, 500}
    };


    for (const auto& enemy : enemies) {
        mGameQuery.prepare("INSERT INTO Enemy (Name, HP, DMG, XPReward) VALUES (?, ?, ?, ?)");
        mGameQuery.addBindValue(enemy[0]); // Name
        mGameQuery.addBindValue(enemy[1]); // HP
        mGameQuery.addBindValue(enemy[2]); // DMG
        mGameQuery.addBindValue(enemy[3]); // XPReward

        //Execute the query
        if (!mGameQuery.exec()) {

            // Check if the error is due to a duplicate entry
            if (mGameQuery.lastError().number() == 1062) // MySQL error code for duplicate entry
            {
                qDebug() << "Skipping duplicate entry for enemy:" << enemy[0].toString();
                mGameQuery.clear(); // Clear the query object for the next iteration
                continue; // Skip to the next iteration
            }
            else
            {
                qDebug() << "Failed to insert enemy:";
                qDebug() << mGameQuery.lastError().text();
            }
        }
        mGameQuery.clear(); // Clear the query object for the next iteration
    }
}

void Manager::printEnemies()
{
    if (!mGameQuery.exec("SELECT * FROM Enemy"))
    {
        qDebug() << "Failed to execute query:" << mGameQuery.lastError().text();

    }

    // Print enemy vars
    std::cout << "List of enemies:" << std::endl;
    while (mGameQuery.next())
    {
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int xpReward = mGameQuery.value("XPReward").toInt();
        qDebug()
        << "Enemy:" << name
        << "HP:" << hp
        << "DMG:" << dmg
        << "XP Reward:" << xpReward;
        std::cout << std::endl;
    }
}

//------------------------------------------------ Hero handling --------------------------------------------------------
void Manager::setHero(Hero newHero)
{
    mHero = newHero;
}

void Manager::saveHero(){

    // Prepare the INSERT query for the Hero table
    QString insertHeroQuery = "INSERT INTO Hero (Name, HP, DMG, Level, XP) VALUES (?, ?, ?, ?, ?)";
    mGameQuery.prepare(insertHeroQuery);

    // Set values for the hero's attributes
    QString heroName = QString::fromStdString(mHero.getName()); //
    int heroHP = mHero.getHP(); //
    int heroDMG = mHero.getDMG(); //
    int heroLevel = mHero.getLevel(); //
    int heroXP = mHero.getCurrentXP(); //

    // Bind values to the parameters
    mGameQuery.addBindValue(heroName); // Name
    mGameQuery.addBindValue(heroHP); // HP
    mGameQuery.addBindValue(heroDMG); // DMG
    mGameQuery.addBindValue(heroLevel); // Level
    mGameQuery.addBindValue(heroXP); // XP

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to insert hero:";
        qDebug() << mGameQuery.lastError().text();
    }
    // Success message
    qDebug() << "Your hero takes a rest at the local inn, and is ready when you return!";
}

Hero Manager::loadHero(int heroID) {
    // Prepare the SELECT query for retrieving the hero from the database based on ID
    QString selectHeroQuery = "SELECT * FROM Hero WHERE ID = ?";
    mGameQuery.prepare(selectHeroQuery);
    mGameQuery.addBindValue(heroID); // Bind the hero's ID as a parameter

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
        return Hero();
    }

    // Check if any results were returned
    if (mGameQuery.next())
    {
        // Extract hero's attributes from the query result
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int level = mGameQuery.value("Level").toInt();
        int xp = mGameQuery.value("XP").toInt();

        // Construct a new instance of the hero using the retrieved attributes
        Hero loadedHero(name.toStdString());
        loadedHero.setHP(hp);
        loadedHero.setDMG(dmg);
        loadedHero.setLevel(level);
        loadedHero.setXP(xp);
        return loadedHero;

    }
    else
    {
        qDebug() << "No hero found with the ID:" << heroID;
        return Hero();
    }
}

void Manager::printHeros()
{
     // Execute the query
    if (!mGameQuery.exec("SELECT * FROM Hero")) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
    }

    // Iterate over the results and print each hero's attributes
    while (mGameQuery.next()) {
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int level = mGameQuery.value("Level").toInt();
        int xp = mGameQuery.value("XP").toInt();

        qDebug() << "Name:" << name << "HP:" << hp << "DMG:" << dmg << "Level:" << level << "XP:" << xp;
    }
}

