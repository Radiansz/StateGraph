#include "screendata.h"



ScreenData::~ScreenData()
{

}

ScreenData::ScreenData():ScreenData(0,0)
{

}

ScreenData::ScreenData(int x, int y):ScreenData(x,y, "DefName", "DefInfo", QColor(40,40,255)){
    this->x = x;
    this->y = y;
}

ScreenData::ScreenData(int x, int y, QString n, QString i, QColor c){
    this->x = x;
    this->y = y;
    name = n;
    info = i;
    color = c;
}

int ScreenData::getX(){
    return x;
}

int ScreenData::getY(){
    return y;
}

QString ScreenData::getName(){
    return name;
}

QString ScreenData::getInfo(){
    return info;
}

QColor ScreenData::getColor(){
    return color;
}


void ScreenData::setX(int nx){
    x = nx;
}

void ScreenData::setY(int ny){
    y = ny;
}

void ScreenData::setName(QString nn){
    name = nn;
}

void ScreenData::setInfo(QString nInf){
    info = nInf;
}

void ScreenData::setColor(QColor nc){
    color = nc;
}

QString ScreenData::serialize(){
    QDomNode el = serializeToDom();
    QDomDocument doc = el.toDocument();
    QDomElement test = el.toElement();
    return doc.toString() ;

}

QDomNode ScreenData::serializeToDom(){
    QString temp;
    QDomDocument doc("");
    QDomElement head = doc.createElement("ScreenData");
    QDomElement node1 = doc.createElement("X");
    QDomNode node2 = doc.createElement("Y");
    QDomText dstr= doc.createTextNode(temp.setNum(x));
    QDomText tstr= doc.createTextNode(temp.setNum(y));
    node1.appendChild(dstr);
    node2.appendChild(tstr);
    head.appendChild(node1);
    head.appendChild(node2);
    doc.appendChild(head);
    return doc;
}

void ScreenData::deserialize(QString data){
    QDomDocument doc;
    doc.setContent(data);
    deserializeFromDom(doc);
}

void ScreenData::deserializeFromDom(QDomNode node){
    QDomNode head = node;
    QDomNode temp;
    if(head.nodeName() == "ScreenData"){
        temp = head.firstChild();
        if( temp.nodeName() == "X"){
            QString text = temp.firstChild().toText().data();
            x = text.toInt();
        }
        temp = temp.nextSibling();
        if( temp.nodeName() == "Y"){
           y = temp.firstChild().toText().data().toInt();
        }
    }
}
