#include "server.h"

//TODO 1 - реализовать модели +-
//TODO 2 - реализовать мидлвары
//TODO 3 - реализовать резервирование  ресурсов(пока без времени)
//TODO 4 - если клиент разорвал соединение, освобождать ресурс(опционально)

Server::Server(){
    //load options from file
    options = *new Options("settings.ini");
    resourceController = new ResourceController(options.resourcesCounter);

    if(this->listen(QHostAddress::Any,  options.port))
        qDebug() << "server started on port " << options.port;
    else
        qDebug()<< "server not started";
    nextBlockSize = 0;
}


void Server::incomingConnection(qintptr socketDescriptor){

    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater );

    sockets.push_back(socket);

    qDebug() << "new client connected";
}

void Server::slotReadyRead(){
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);
    QString requestString;


//    auto getIp = [this](){
//       QTcpSocket* new_socket = QTcpServer::nextPendingConnection();
//       return new_socket->peerAddress().toString();
//    };

    if(in.status() == QDataStream::Ok){
        qDebug() << "reading data from stream...";
        for(;;){
            if(nextBlockSize == 0){
                if(socket->bytesAvailable() < 2)
                    break;
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
                break;

            in >> requestString;
            nextBlockSize = 0;
            break;
        }
    }
    else{
        qDebug() << "DataStream error";
    }
    qDebug()<< "request string = " + requestString;
    //TODO: cast json request to Object of "Request"
    Request request(requestString);
    Response response;
    //TODO: middlewares here
    qDebug() << "Request unparsed sucessfully";

    //Check Json is valid

    if(!usernameAccepted(request.username, options.acceptedUsers)){
        SendToClient("acess for user " + request.username + " denied", socket);
//        bannedIps.push_back(getIp());
        return;
    }

//    if(!ipNotBanned(getIp())){
//        SendToClient("acess denied", socket);
//        return;
//    }
//    qDebug() << "ip not Banned";

    //TODO: first check user is not already connected
    User *connectedUser = new User(request.username);
    connectedUsers.push_back(connectedUser);


    //check is this connect requst or resource request
    if(request.resources.size() <= 0){
       SendToClient("connected sucessfully", socket);
    }
    //elseif "first you need to login"
    else{
        qDebug() << "try take resource";
        Response response;
        response.username = request.username;
        quint32 iterator = 0;

        for(auto resource: request.resources){
            response.resource = iterator+1;

            //проверка на существование ресурса с таким индексом
            if(iterator >= resourceController->resources.size()){
               response.status = 0;
               SendToClient(response.toJsonString(), socket);
            }
            else{
                if(resource>0){
                    if(resourceController->resources[iterator]->ResorceRequest(connectedUser, request.time, options.maxReserveTime))
                         response.status = 1;
                    else
                        response.status = 0;

                    SendToClient(response.toJsonString(), socket);
                }

            }
            iterator++;
            //TODO request for resources and responses
        }
    }




    //end of middlewares

}

//send response to one client
void Server::SendToClient(QString str, QTcpSocket *socket){
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}

//send response to all clients
void Server::SendToAllClients(QString str){
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    for(auto socket: sockets){
        socket->write(Data);
    }
}


//Middlewares

bool Server::usernameAccepted(QString username, QStringList acceptedUsers){
    for(QString user: acceptedUsers){
        if(user == username){
            return true;
        }
    }
    return false;
};
//bool Server::ipNotBanned(QString socketIp){
//    for(QString ip: bannedIps){
//        if(ip == socketIp)
//            return false;
//    }
//    return true;
//};

