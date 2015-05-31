#include "paintwidget.h"

PaintWidget::PaintWidget() : PaintWidget(0)
{
}



PaintWidget::PaintWidget(QWidget *parent):QWidget(parent),pick(QColor(255,40,40)),unpick(QColor(40,40,255)){
    graph = new States::StateDiagram<ScreenData>();
    graph->addState(new States::State<ScreenData>(ScreenData(30,40)));
    graph->addState(new States::State<ScreenData>(ScreenData(150,40)));
    graph->addState(new States::State<ScreenData>(ScreenData(90,150)));
    graph->addConnection(0,1);
    graph->addConnection(1,2);
    creatingState = false;
    movingState = false;
    creatingConn = false;
    pickingPath = false;
    movingScreen = false;
    stage2 = false;

    offsetX = 0;
    offsetY = 0;

    oldX = 0;
    oldY = 0;

    choosedState = 0;

}

PaintWidget::~PaintWidget()
{

}
//Сигналы
void PaintWidget::newState(){
    creatingState = true;
}
void PaintWidget::newConnection(){
    creatingConn = true;
}

void PaintWidget::openFile(QString file){
    int i =2;
}

void PaintWidget::saveFile(QString file){

}


bool PaintWidget::isBusy(){
    return creatingState && movingState && creatingConn && pickingPath && movingScreen;
}
//Поиск состояния по координатам
States::State<ScreenData>* PaintWidget::findState(int x, int y){
    for(States::StateDiagram<ScreenData>::iterator i = graph->begin(); !i.isEnd() ; i++){
        ScreenData sd = (*i)->getData();
        if(x >= sd.getX()&& x <= sd.getX() + 100 && y >= sd.getY() && y <= sd.getY() + 80){
            return *i;
        }
    }
    return 0;
}

QPoint PaintWidget::findPoint(States::State<ScreenData>* s1,States::State<ScreenData>* s2){
    float rad = 64;
    ScreenData sd1 = s1->getData();
    ScreenData sd2 = s2->getData();
    QVector2D direct(sd2.getX()-sd1.getX(),sd2.getY()-sd1.getY());
    direct.normalize();
    int tx = (direct.x()*rad) + sd1.getX() + 50,ty = (direct.y()*rad) + sd1.getY() + 40;
    return QPoint(tx - offsetX,ty - offsetY);
}

void PaintWidget::pickState(States::State<ScreenData>* s){
    ScreenData sd = s->getData();
    sd.setColor(pick);
    s->setData(sd);
    choosedState = s;
}

void PaintWidget::unpickState(States::State<ScreenData>* s){
    if(s){
        ScreenData sd = s->getData();
        sd.setColor(unpick);
        s->setData(sd);
    }
}

void PaintWidget::mousePressEvent(QMouseEvent* e){
    int x = e->x() + offsetX, y = e->y() + offsetY;
    if(creatingConn){
        States::State<ScreenData> *st = findState(x,y);
        if(!stage2){
            unpickState(choosedState);
            pickState(st);
            stage2 = true;
        }
        else
        {
            graph->addConnection(choosedState,st);
            stage2 = false;
            creatingConn = false;
        }
    }

    if(!isBusy()){
        States::State<ScreenData> *st = findState(x,y);
        if(st == 0){
            movingScreen = true;
        }
        else{
            movingState = true;
            unpickState(choosedState);
            pickState(st);
        }
        oldX = e->x();
        oldY = e->y();
        update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* e){
    int x = e->x() + offsetX, y = e->y() + offsetY;
    if(creatingState){
        graph->addState(new States::State<ScreenData>(ScreenData(x,y)));
        creatingState = false;
        update();
        return;
    }
    if(movingScreen){
        movingScreen = false;
        update();
    }
    if(movingState){
        movingState = false;
        update();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent* e){
    int x = e->x() + offsetX, y = e->y() + offsetY;
    if(movingScreen){
        offsetX += (oldX - e->x());
        offsetY += (oldY - e->y());
        oldX =  e->x();
        oldY =  e->y();
        update();
    }
    if(movingState){
        ScreenData sd = choosedState->getData();
        int tX = sd.getX(), tY = sd.getY();
        sd.setX(tX - (oldX - e->x()));
        sd.setY(tY - (oldY - e->y()));
        choosedState->setData(sd);
        oldX =  e->x();
        oldY =  e->y();
        update();
    }
}
void PaintWidget::drawConnection(States::State<ScreenData>* s1, States::State<ScreenData>* s2 , QPainter &p){
    p.setPen(unpick);
    p.drawLine(findPoint(s1,s2),findPoint(s2,s1));
}

void PaintWidget::drawState(States::State<ScreenData>* s, QPainter &p){
    ScreenData d = s->getData();
    QString str = d.getName();
    int x = d.getX() - offsetX, y = d.getY() - offsetY;
    p.setPen(d.getColor());
    p.drawRect(x,y,100,80);
    p.drawRect(x,y,100,15);
    p.setPen(QColor(255,255,255));
    p.drawText(QPoint(x+5,y+14),str);
}

void PaintWidget::paintEvent(QPaintEvent * z){
    QPainter p(this);
    p.fillRect(0,0,width(),height(),QBrush(QColor(0,0,0),Qt::SolidPattern));
    for(States::StateDiagram<ScreenData>::iterator i = graph->begin(); !i.isEnd() ; i++){
        drawState(*i,p);
        for(States::StateDiagram<ScreenData>::iterator j = graph->getConnections(*i); !j.isEnd(); ++j){
            drawConnection(*i,*j, p);
        }
    }
}

