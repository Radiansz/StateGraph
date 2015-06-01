#include "paintwidget.h"

PaintWidget::PaintWidget() : PaintWidget(0)
{
}



PaintWidget::PaintWidget(QWidget *parent):QWidget(parent),pick(QColor(255,40,40)),unpick(QColor(40,40,255)),pathColor(QColor(40,255,40)){
    graph = new States::StateDiagram<ScreenData>();

    pathDraw = false;
    pathSrc = 0;
    pathDst = 0;
    offAll();

    offsetX = 0;
    offsetY = 0;

    k = 1.0f;

    oldX = 0;
    oldY = 0;

    choosedState = 0;

}

PaintWidget::~PaintWidget()
{

}
//**************************************
//Слоты
//**************************************
void PaintWidget::newState(){
    offAll();
    creatingState = true;
    ordinary = true;
}
void PaintWidget::newIState(){
    offAll();
    creatingState = true;
    initial = true;
}

void PaintWidget::newFState(){
    offAll();
    creatingState = true;
    final = true;
}

void PaintWidget::newSState(){
    offAll();
    creatingState = true;
    super = true;
}

void PaintWidget::newConnection(){
    offAll();
    creatingConn = true;
}

void PaintWidget::delState(){
    if(choosedState != 0){
        graph->removeState(choosedState);
        choosedState = 0;
    }
    update();
}

void PaintWidget::delConnection(){
    offAll();
    removeCon = true;
}

void PaintWidget::newPath(){
    offAll();
    pickingPath = true;

}

void PaintWidget::openFile(QString file){
    States::StateDiagram<ScreenData>* testG = new States::StateDiagram<ScreenData>();
    try{
        testG->deserializeFromFile(file);
    }
    catch(States::BadXMLException e){
        return;
    }
    delete testG;
    choosedState = 0;
    pathDst = 0;
    pathSrc = 0;
    graph->deserializeFromFile(file);
}

void PaintWidget::saveFile(QString file){
   graph->serializeToFile(file);

}
void PaintWidget::newName(QString name){
    if(choosedState){
        ScreenData sd = choosedState->getData();
        sd.setName(name);
        choosedState->setData(sd);
        update();
    }
}

void PaintWidget::newInfo(QString info){
    if(choosedState){
        ScreenData sd = choosedState->getData();
        sd.setInfo(info);
        choosedState->setData(sd);
        update();
    }
}

//**************************************


bool PaintWidget::isBusy(){
    return creatingState || movingState || creatingConn || pickingPath || movingScreen || removeState || removeCon;
}
//Поиск состояния по координатам
States::State<ScreenData>* PaintWidget::findState(int x, int y){
    int w = 70, h =50;
    for(States::StateDiagram<ScreenData>::iterator i = graph->begin(); !i.isEnd() ; i++){
        ScreenData sd = (*i)->getData();
        States::SuperState<ScreenData>* test = dynamic_cast<States::SuperState<ScreenData>*>(*i);
        if(test){
            w = 210;
            h= 150;
        }
        else{
            w = 70;
            h= 50;
        }
        if(x >= sd.getX()&& x <= sd.getX() + w && y >= sd.getY() && y <= sd.getY() + h){
            return *i;
        }
    }
    return 0;
}


void PaintWidget::offAll(){
    creatingState = false;
    movingState = false;
    creatingConn = false;
    pickingPath = false;
    movingScreen = false;
    stage2 = false;
    initial = false;
    final = false;
    super = false;
    ordinary = false;
    removeState = false;
    removeCon = false;
}

void PaintWidget::pickState(States::State<ScreenData>* s){
    if(s!=0){
        ScreenData sd = s->getData();
        sd.setColor(pick);
        s->setData(sd);
        choosedState = s;
        emit nameChng(sd.getName());
        emit infoChng(sd.getInfo());
    }
}

void PaintWidget::unpickState(States::State<ScreenData>* s){
    if(s){
        ScreenData sd = s->getData();
        sd.setColor(unpick);
        s->setData(sd);
    }
}

void PaintWidget::mouseDoubleClickEvent(QMouseEvent * e){
    int x = e->x() + offsetX, y = e->y() + offsetY;
    if(!isBusy()){
        States::State<ScreenData> *st = findState(x,y);
        if(st){
            States::SuperState<ScreenData>* test = dynamic_cast<States::SuperState<ScreenData>*>(st);
            if(test){
                MainWindow* mw = new MainWindow((QWidget*)this->parent(),test->getInnerGraph());
                mw->show();
            }
        }

    }
}


void PaintWidget::mousePressEvent(QMouseEvent* e){
    int x = e->x() + offsetX, y = e->y() + offsetY;
    //Добавление удаление связи
    if(creatingConn || removeCon || pickingPath){
        States::State<ScreenData> *st = findState(x,y);
        if(st){
            if(!stage2){
                unpickState(choosedState);
                pickState(st);
                if(pickingPath)
                    pathSrc = st;
                stage2 = true;
            }
            else
            {
                if(creatingConn)
                    graph->addConnection(choosedState,st);
                else{
                    if(pickingPath)
                        pathDst = st;
                    else
                        graph->removeConnection(choosedState,st);
                }
                stage2 = false;
                creatingConn = false;
                removeCon = false;
                pickingPath = false;
            }
        }
    }
    //Выбор/начало перемещения состояния или начало перемещения экрана
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
    // Создание одного из состояний
    if(creatingState){
        if(initial){
            graph->setInitialState(new States::State<ScreenData>(ScreenData(x,y)));
            initial = false;
        }
        if(final){
            graph->setFinalState(new States::State<ScreenData>(ScreenData(x,y)));
            final = false;
        }
        if(super){
            States::SuperState<ScreenData>* ss = new States::SuperState<ScreenData>(ScreenData(x,y));
            ss->setInnerGraph(new States::StateDiagram<ScreenData>());
            graph->addState(ss);
            super = false;

        }
        if(ordinary){
            graph->addState(new States::State<ScreenData>(ScreenData(x,y)));
            ordinary = false;
        }
        creatingState = false;
        update();
        return;
    }
    // Окончание перемещения экрана
    if(movingScreen){
        movingScreen = false;
        update();
    }
    // Окончание перемещения состояния
    if(movingState){
        movingState = false;
        update();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent* e){
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
// Отрисовка
QPoint PaintWidget::findPoint(States::State<ScreenData>* s1,States::State<ScreenData>* s2){
    float rad = 45;
    int xGap = 35, yGap = 25;
    ScreenData sd1 = s1->getData();
    ScreenData sd2 = s2->getData();
    States::SuperState<ScreenData>* test = dynamic_cast<States::SuperState<ScreenData>*>(s1);
    if(test){
        xGap = 90;
        yGap = 60;
        rad = 108;
    }
    QVector2D direct(sd2.getX()-sd1.getX(),sd2.getY()-sd1.getY());
    direct.normalize();
    int tx = (direct.x()*rad) + sd1.getX() + xGap,ty = (direct.y()*rad) + sd1.getY() + yGap;
    return QPoint(tx*k - offsetX,ty*k - offsetY);
}

void PaintWidget::drawConnection(States::State<ScreenData>* s1, States::State<ScreenData>* s2,QPainter &p){
    if(pathDraw)
        p.setPen(pathColor);
    else
        p.setPen(unpick);
    QPoint p1 = findPoint(s1,s2), p2 = findPoint(s2,s1);
    p.drawLine(p1,p2);
    drawArrow(p1,p2,p);
}

void PaintWidget::drawArrow(const QPoint& p1, const QPoint& p2,QPainter &p ){
    int leng = 10;
    int angle = 70;
    QVector2D dir(p1.x() - p2.x(), p1.y() - p2.y());
    dir.normalize();
    QVector2D lDir(dir.x()*qCos(angle) - dir.y()*qSin(angle), dir.x()*qSin(angle) + dir.y()*qCos(angle) );
    QVector2D rDir(dir.x()*qCos(-angle) - dir.y()*qSin(-angle), dir.x()*qSin(-angle) + dir.y()*qCos(-angle) );
    QPoint l(leng*k*lDir.x() + p2.x(), leng*k*lDir.y() + p2.y());
    QPoint r(leng*k*rDir.x() + p2.x(), leng*k*rDir.y() + p2.y());
    p.drawLine(p2,l);
    p.drawLine(p2,r);
}

void PaintWidget::drawState(States::State<ScreenData>* s,States::StateDiagram<ScreenData>* graph, QPainter &p){


    int testt = 100*k;
    int stW = 70*k, stH= 50*k, stlH = 15*k , rad = 30*k;
    ScreenData d = s->getData();
    QString str = d.getName();
    int x = d.getX()*k - offsetX, y = d.getY()*k - offsetY;
    if(pathDraw)
        p.setPen(pathColor);
    else
        p.setPen(d.getColor());
    if(graph->isInitialState(s)){
        p.drawEllipse(x+15*k,y+5*k,rad,rad);
        return;
    }
    if(graph->isFinalState(s)){
        p.drawEllipse(x+15*k,y+5*k,rad,rad);
        p.drawEllipse(x+stW/8+15*k,y + stH/8 +5*k,rad/2,rad/2);
        return;
    }
    States::SuperState<ScreenData>* test = dynamic_cast<States::SuperState<ScreenData>*>(s);
    if(test != 0 && graph==this->graph){
        p.drawRect(x,y,210,150);
        p.drawRect(x,y,210,15);
        States::StateDiagram<ScreenData>* tgraph = test->getInnerGraph();
        if(tgraph){
            int tOX = offsetX, tOY = offsetY;
            p.setClipRect(x+1,y+16,209,134);
            k = 134/float(width());
            offsetX = -x; offsetY = -y;
            p.setClipping(true);
            drawGraph(tgraph,p);
            p.setClipping(false);
            offsetX = tOX;
            offsetY = tOY;\
            k = 1.0f;
        }
        //p.drawRect(x+20,y+30,60,50);
        p.setPen(QColor(255,255,255));
        p.drawText(QPoint(x+5,y+14),str);
        return;
    }
    p.drawRect(x,y,stW,stH);
    p.drawRect(x,y,stW,stlH);
    if(k>=1.0f){
        p.setPen(QColor(255,255,255));
        p.setFont(QFont("Arial",7,2));
        p.drawText(QPoint(x+5,y+14),str);
    }


}

void PaintWidget::drawGraph(States::StateDiagram<ScreenData>* graph, QPainter &p){
    p.fillRect(0,0,width(),height(),QBrush(QColor(0,0,0),Qt::SolidPattern));
    for(States::StateDiagram<ScreenData>::iterator i = graph->begin(); !i.isEnd() ; i++){
        drawState(*i, graph, p );
        for(States::StateDiagram<ScreenData>::iterator j = graph->getConnections(*i); !j.isEnd(); ++j){
            drawConnection(*i,*j, p);
        }
    }
}

void PaintWidget::drawPath(QPainter &p){
    if(pathSrc == 0 || pathDst == 0)
        return;
    pathDraw = true;
    States::StateDiagram<ScreenData>::iterator i = graph->findPath(pathSrc,pathDst);
    if(!i.isEnd()){
        States::State<ScreenData>* temp = *i;
        drawState(temp,graph,p);
        for(; !i.isEnd(); ++i){
            drawState(temp,graph,p);
            drawConnection(temp,*i,p);
            temp = *i;
        }
        drawState(temp,graph,p);
    }
    pathDraw = false;
}

void PaintWidget::paintEvent(QPaintEvent *){
    QPainter p(this);
    drawGraph(graph,p);
    drawPath(p);
}



void PaintWidget::setGraph(States::StateDiagram<ScreenData>* d){
    graph = d;
}
