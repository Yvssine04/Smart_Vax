#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;

    // Remove any existing database connection to prevent conflicts
    if (QSqlDatabase::contains("QODBC")) {
        QSqlDatabase::removeDatabase("QODBC");
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projetc++"); // Make sure this matches your ODBC DSN
    db.setUserName("Mariem");
    db.setPassword("2004");

    if (db.open()) {
        test = true;
        qDebug() << "Database connection successful!";
    } else {
        qDebug() << "Database connection failed!";
        qDebug() << "Error: " << db.lastError().text(); // Print the exact error message
    }

    return test;
}
