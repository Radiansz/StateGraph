#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <mainwindow.h>
#include <state.h>
#include <superstate.h>
#include <screendata.h>
#include <stategraph.h>
#include <QPoint>
#include <QMouseEvent>
#include <badxmlexception.h>
class PaintWidget : public QWidget
{
Q_OBJECT
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent *);
public:
    PaintWidget();
    PaintWidget(QWidget *parent);
    virtual ~PaintWidget();
    void setGraph(States::StateDiagram<ScreenData>* d);
public slots:
    void newState();
    void newIState();
    void newFState();
    void newSState();
    void newConnection();
    void delState();
    void delConnection();
    void newName(QString);
    void newInfo(QString);

    void openFile(QString);
    void saveFile(QString);
signals:
    void nameChng(QString);
    void infoChng(QString);
private:
    void drawState(States::State<ScreenData>*, QPainter &);
    void drawConnection(States::State<ScreenData>* s1, States::State<ScreenData>* s2 , QPainter &p);
    States::State<ScreenData>* findState(int x, int y);
    QPoint findPoint(States::State<ScreenData>*,States::State<ScreenData>*);
    void offAll();

    void pickState(States::State<ScreenData>*);
    void unpickState(States::State<ScreenData>*);

    bool isBusy();

    const QColor unpick;
    const QColor pick;

    States::StateDiagram<ScreenData>* graph;
    States::State<ScreenData>* choosedState;



    int offsetX;
    int offsetY;

    int oldX;
    int oldY;




    bool creatingState;
    bool initial;
    bool final;
    bool super;
    bool ordinary;
    bool movingState;
    bool creatingConn;
    bool pickingPath;
    bool removeState;
    bool removeCon;

    bool movingScreen;
    bool stage2;

};

#endif // PAINTWIDGET_H
