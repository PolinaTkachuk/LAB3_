#ifndef DATABASEJSON_H
#define DATABASEJSON_H
#include <QtSql>
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QPair>
#include <QVector>

using Data_=QPair<QString, double>;//из контейнерa берем
using Containerdata_ = QVector< QPair<QString, double> >;

//общий интерфейс - получ данных абстрактного типа
class Idata
{
    virtual Containerdata_ getData(const QString&) //получение данных(еще нет типа) с помощью контейнера
    {
        return QVector<Data_>{};
    }
};

//Наследуем 2 класса для бд и json

//для данных из бд Sql
class dataSql : public Idata
{
public:
    Containerdata_ getData (const QString&) override;
};

//для данных из файлов формата JSON
class dataJSON: public Idata
{
public:
    Containerdata_ getData (const QString&) override;
};





#endif // DATABASEJSON_H
