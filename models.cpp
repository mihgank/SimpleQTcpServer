#include "models.h"
#include "utils.h"

#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>

//Options Class
Options::Options(){};
Options::Options(QString settingsPath){

    QSettings settings(":/"+settingsPath, QSettings::IniFormat);

    this->acceptedUsers = settings.value("settings/users").toStringList();
    this->maxUsers = settings.value("settings/max_users").toInt();
    this->port = settings.value("settings/ports").toStringList()[0].toInt();
    this->maxReserveTime = settings.value("settings/maxReserveTime").toInt();
    this->resourcesCounter = settings.value("settings/resourcesCounter").toInt();
};

//UserClass
User::User(QString _username){
    this->username = _username;
}

//TODO:Resource Class
Resource::Resource(quint32 _index){
    this->index = _index;
}
bool Resource::ResorceRequest(User *user, quint32 timeToReserve, quint32 maxReserveTime){
    if(this->ReservedByUser == nullptr){
        this->ReservedByUser = user;
        this->reserveStartAt = utils::GetCurrentTime();
        this->reserveTime = timeToReserve;
        return true;
    }
    if((currentReservedTime() > maxReserveTime)||(currentReservedTime() > this->reserveTime)){
        this->ReservedByUser = user;
        this->reserveStartAt = utils::GetCurrentTime();
        this->reserveTime = timeToReserve;
        return true;
    }
    return false;

}
quint32 Resource::currentReservedTime(){
    return utils::GetCurrentTime() - this->reserveStartAt;
}



//Resource Controller Class
ResourceController::ResourceController(quint32 resCount){
    for(quint32 i = 0; i < resCount; i++){
        resources.push_back(new Resource(i));
    }
};




//Request Class
 Request::Request(QString jsonData){
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8(), &err);
    qDebug() << &err;
    QJsonObject obj = doc.object();
    QJsonValue requestedResources;

    if(doc.isEmpty()){
        qDebug() << "document empty";
    }
    if(!doc.isObject()){
        qDebug() << "document isn't object";
    }


   this->username = obj.value("username").toString();
   this->time = obj.value("time").toInt();
   requestedResources = obj.value("resource");


   auto loop_str = requestedResources.toString().toStdString();
   //request byte unparse
   for(auto c: loop_str){
    if(c != '0')
        this->resources.push_back(1);
    else
        this->resources.push_back(0);
   }

 }


 //Response Class
 QString Response::toJsonString(){
    QJsonObject jsonObj;

    jsonObj["username"] = this->username;
    jsonObj["resource"] = this->resource;
    jsonObj["status"] = this->status;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
 }
