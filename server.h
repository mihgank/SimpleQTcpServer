#ifndef SERVER_H
#define SERVER_H

#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

#include "models.h"

class Server: public QTcpServer{
    Q_OBJECT

public:
    Server();
    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> sockets;
    QByteArray Data;
    quint16 nextBlockSize;
    Options options;
    QVector <QString> bannedIps;
    QVector <User*> connectedUsers;
    ResourceController *resourceController;

    //request methods
    void SendToAllClients(QString str);
    void SendToClient(QString str, QTcpSocket *socket);

    //middlewares
    //TODO: реализовать проверку запроса на корректность JSON'a
    bool isRequestValid(Request request);
    bool usernameAccepted(QString username, QStringList acceptedUsers);
    bool ipNotBanned(QString ip);

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // SERVER_H
