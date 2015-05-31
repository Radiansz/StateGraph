#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <state.h>
#include <screendata.h>
#include <stategraph.h>
#include <QPoint>
#include <QMouseEvent>
class PaintWidget : public QWidget
{
Q_OBJECT
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
public:
    PaintWidget();
    PaintWidget(QWidget *parent);
    virtual ~PaintWidget();
public slots:
    void newState();
    void newConnection();

    void openFile(QString);
    void saveFile(QString);
private:
    void drawState(States::State<ScreenData>*, QPainter &);
    void drawConnection(States::State<ScreenData>* s1, States::State<ScreenData>* s2 , QPainter &p);
    States::State<ScreenData>* findState(int x, int y);
    QPoint findPoint(States::State<ScreenData>*,States::State<ScreenData>*);

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
    bool movingState;
    bool creatingConn;
    bool pickingPath;

    bool movingScreen;
    bool stage2;

};

#endif // PAINTWIDGET_H
