#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL");
    DB.setHostName("localhost"); // Currently set to localhost
    DB.setDatabaseName("Game"); // Change to desired schema
    DB.setUserName("sammy"); // Change to username
    DB.setPassword("password"); // Change to password
    DB.open();
    QSqlQuery gameQuery;

    QString createDB = "CREATE DATABASE IF NOT EXISTS Game";
    gameQuery.exec(createDB);

    QString createTB1 = "CREATE TABLE IF NOT EXISTS Hero ("
            "ID INT AUTO_INCREMENT PRIMARY KEY,"
            "Name VARCHAR(255) UNIQUE,"
            "HP INT,"
            "DMG INT,"
            "Level INT,"
            "XP INT"
            ")";
    gameQuery.exec(createTB1);

    QString createTB2 = "CREATE TABLE IF NOT EXISTS Enemy ("
            "ID INT AUTO_INCREMENT PRIMARY KEY,"
            "Name VARCHAR(255) UNIQUE,"
            "HP INT,"
            "DMG INT,"
            "XPReward INT"
            ")";
    gameQuery.exec(createTB2);


    QString showDB = "show schemas";
    gameQuery.exec(showDB);


    return 1;
}

/*
query.exec("SELECT * FROM employee");
while (query.next()) {
    QString fname  = query.value(0).toString();
    QString lname = query.value(1).toString();
    int ssn = query.value(2).toInt();
    qDebug() << "Fname:" << fname << "Lname:" << lname << "SSN:" << ssn;
    }
qDebug() << "Finished";
*/
