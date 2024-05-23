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

        // Ensure the connection is set to the "Game" database
        if (mDB.databaseName() != "Game") {
            if (!mDB.isOpen()) {
                qDebug() << "Failed to select database:";
                qDebug() << mDB.lastError().text();
            } else {
                mDB.setDatabaseName("Game");
            }
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
void Manager::setEnemy(Enemy newEnemy)
{
    mEnemy = newEnemy;
}

void Manager::addEnemies()
{
    //After DB creation, insert enemies
    QList<QVariantList> enemies = {
        {"Weak", 2, 1, 500},
        {"Medium", 5, 2, 1000},
        {"Strong", 10, 5, 5000},
        {"Very Strong", 50, 20, 10000},
        {"DEATH", 1000, 1000, 1}
    };


    for (const auto& enemy : enemies) {
        mGameQuery.prepare("INSERT INTO Enemy (Name, HP, DMG, XPReward) VALUES (?, ?, ?, ?)");
        mGameQuery.addBindValue(enemy[0]); // Name
        mGameQuery.addBindValue(enemy[1]); // HP
        mGameQuery.addBindValue(enemy[2]); // DMG
        mGameQuery.addBindValue(enemy[3]); // XPReward

        //Execute the query
        if (!mGameQuery.exec()) {

            //Skip duplicate entires (Database and table already exsists)
            if (mGameQuery.lastError().number() == 1062) // MySQL error code for duplicate entry
            {
                //Output MSG for duplicates
                //qDebug() << "TEST - Skipping duplicate entry for enemy:" << enemy[0].toString();
                mGameQuery.clear(); // Clear the query object for the next iteration
                continue; // Skip to the next iteration
            }
            else
            {
                //Other error than 1062???
                qDebug() << "Failed to insert enemy:";
                qDebug() << mGameQuery.lastError().text();
            }
        }
        mGameQuery.clear(); // Clear the query object for the next iteration
    }
}

Enemy Manager::loadEnemy(int enemyID) {

    // Prepare the SELECT query for retrieving the enemy from the database based on ID
    QString selectEnemyQuery = "SELECT * FROM Enemy WHERE ID = ?";
    mGameQuery.prepare(selectEnemyQuery);
    mGameQuery.addBindValue(enemyID); // Bind the enemy's ID as a parameter

    // Execute the query
    if (!mGameQuery.exec()) {
        qDebug() << "Failed to execute select query:";
        qDebug() << mGameQuery.lastError().text();
        throw std::runtime_error("Failed to execute select query");
    }

    // Check if any results were returned
    if (mGameQuery.next())
    {
        // Extract hero's attributes from the query result
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int xpreward = mGameQuery.value("XPReward").toInt();


        // Construct a new instance of the hero using the retrieved attributes
        Enemy loadedEnemy(name.toStdString());
        loadedEnemy.setHP(hp);
        loadedEnemy.setDMG(dmg);
        loadedEnemy.setXPReward(xpreward);
        return loadedEnemy;

    }
    else
    {
        throw std::runtime_error("No enemy found with the specified ID");
    }

}

void Manager::printEnemies()
{
    if (!mGameQuery.exec("SELECT * FROM Enemy"))
    {
        qDebug() << "Failed to execute query:" << mGameQuery.lastError().text();

    }

    // Print enemy vars
    while (mGameQuery.next())
    {
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int xpReward = mGameQuery.value("XPReward").toInt();
        qDebug()
        << "ID:" << id
        << "Enemy:" << name
        << "HP:" << hp
        << "DMG:" << dmg
        << "XP Reward:" << xpReward;
    }
}

void Manager::printEnemyStats()
{
    std::cout << "---- Current Enemy ----" << std::endl;
    std::cout << "Enemy name: " << mEnemy.getName() << std::endl
              << "HP: " << mEnemy.getHP() << std::endl
              << "DMG: " << mEnemy.getDMG() << std::endl
              << "XP Reward: " << mEnemy.getXPReward() << std::endl;
}

//------------------------------------------------ Hero handling --------------------------------------------------------
void Manager::setHero(Hero newHero)
{
    mHero = newHero;
}

void Manager::saveHero(){

    // Prepare the INSERT query for the Hero table
    QString insertHeroQuery = "INSERT INTO Hero (Name, HP, DMG, Level, XP) "
                              "VALUES (:Name, :HP, :DMG, :Level, :XP)"
                              "ON DUPLICATE KEY UPDATE "
                              "HP = :HP,"
                              "DMG = :DMG,"
                              "Level = :Level,"
                              "XP = :XP";
    mGameQuery.prepare(insertHeroQuery);

    // Bind values to the parameters
    mGameQuery.bindValue(":Name", QString::fromStdString(mHero.getName()));
    mGameQuery.bindValue(":HP", mHero.getHP());
    mGameQuery.bindValue(":DMG", mHero.getDMG());
    mGameQuery.bindValue(":Level", mHero.getLevel());
    mGameQuery.bindValue(":XP", mHero.getCurrentXP());

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
        throw std::runtime_error("Failed to execute select query");
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
        throw std::runtime_error("No hero found with the specified ID");
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
        int id = mGameQuery.value("ID").toInt();
        QString name = mGameQuery.value("Name").toString();
        int hp = mGameQuery.value("HP").toInt();
        int dmg = mGameQuery.value("DMG").toInt();
        int level = mGameQuery.value("Level").toInt();
        int xp = mGameQuery.value("XP").toInt();

        qDebug()
        << "ID:" << id
        << "Name:" << name
        << "HP:" << hp
        << "DMG:" << dmg
        << "Level:" << level
        << "XP:" << xp;
    }
}

void Manager::printHeroStats()
{
    std::cout << "---- Current Hero ----" << std::endl;
    std::cout << "Enemy name: " << mHero.getName() << std::endl
              << "HP: " << mHero.getHP() << std::endl
              << "DMG: " << mHero.getDMG() << std::endl
              << "Level: " << mHero.getLevel() << std::endl
              << "XP Reward: " << mHero.getCurrentXP() << std::endl;
}


//------------------------------------------------ Fighting -------------------------------------------------------------
void Manager::nextPhase()
{
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

int Manager::Encounter()
{

    //Return for state handling:
    //Hero won: 10
    //Enemy won: 20
    //Error: -1
    //std::cout << "Test: Try encounter" << std::endl;

    while(mHero.isAlive() && mEnemy.isAlive()){

        //std::cout << "Test: Enter fight loop" << std::endl;
        printHeroStats();
        printEnemyStats();

        nextPhase();
        mEnemy.takeDMG(mHero.getDMG());
        if(!mEnemy.isAlive()){
            //std::cout << "Test: Enemy dead" << std::endl;
            mHero.resetHero();
            mHero.addXP(mEnemy.getXPReward());
            return 10;
        }
        mHero.takeDMG(mEnemy.getDMG());
        if(!mHero.isAlive()){
            mHero.resetHero();
            return 20;
        }

    }
    return -1;
}
