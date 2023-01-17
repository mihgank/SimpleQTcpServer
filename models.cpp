#include "models.h"

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
    this->resourcesCounter = settings.value("settings/maxReserveTime").toInt();
};

//UserClass
User::User(QString _username){
    this->username = _username;
}

//TODO:Resource Class
Resource::Resource(quint32 _index){
    this->index = _index;
}
bool Resource::ResorceRequest(User *user){
    if(this->ReservedByUser == nullptr){
       this->ReservedByUser = user;
        //TODO: working with time;
       return true;
    }
    else{
        //TODO: check if time < Z(maxTime)
        return false;
    }

}
//Resource Controller Class
ResourceController::ResourceController(quint32 resCount){
    for(quint32 i = 0; i < resCount; i++){
        resources.push_back(new Resource(i));
    }
};




//Request Class
 Request::Request(QString jsonData){


    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject obj = doc.object();
    QJsonValue requestedResources;



    this->username = obj.value("username").toString();
     qDebug() << "parsed name: " << obj.value("username").toString();

   this->time = obj.value("time").toInt();

   requestedResources = obj.value("resourse");
   //request byte unparse
   for(quint8 res: requestedResources.toString().toStdString()){
    if(res > 0)
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
