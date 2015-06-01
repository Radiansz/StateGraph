#ifndef SCREENDATA_H
#define SCREENDATA_H
#include "QString"
#include "QDomDocument"
#include "QDomNode"
#include "QDomElement"
#include "QDomText"
#include "QColor"
#include "badxmlexception.h"

class ScreenData
{
    int x;
    int y;
    QString name;
    QString info;
    QColor color;
public:
    ScreenData();
    ScreenData(int x, int y);
    ScreenData(int x, int y, QString n, QString i, QColor c);
    ~ScreenData();
    QString serialize();
    QDomNode serializeToDom();
    void deserialize(QString data);
    void deserializeFromDom(QDomNode node);

    int getX();
    int getY();
    QString getName();
    QString getInfo();
    QColor getColor();

    void setX(int);
    void setY(int);
    void setName(QString);
    void setInfo(QString);
    void setColor(QColor);





};

#endif // SCREENDATA_H
