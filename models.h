#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QStringList>

class Options{
public:
    QStringList acceptedUsers;
    quint32 maxUsers = 20, port = 8000, resourcesCounter = 4;
    quint64 maxReserveTime = 2*60*60;

    Options();
    Options(QString settingsPath);
    //TODO: реализовать метод
    void setPort(QString port);
};

class User{
public:
    QString username;
    User(QString username);
};

class Resource{
public:
    bool ResorceRequest(User *user, quint32 timeToReserve, quint32 maxReserveTime);
    quint32 currentReservedTime();
    Resource(quint32 index);
private:
    quint32 index = 0;
    quint64 reserveTime = 0; //time in seconds for reserve
    quint64 reserveStartAt = 0; //time when reserve started
    User *ReservedByUser = nullptr;
};

class ResourceController{
public:
    QVector<Resource*> resources;
    ResourceController(quint32 resCount);
};

//запрос
class Request{
public:
    QString username;
    quint64 time;
    QVector<quint8> resources;

    //объект заполняется данными при инициализации, передать JSON строку в конструктор
    Request(QString jsonData);
};

//ответ
class Response{
public:
    QString username;
    quint16 resource, status;

    QString toJsonString();
};


#endif // MODELS_H
