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
    offAll();

    offsetX = 0;
    offsetY = 0;

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

void PaintWidget::openFile(QString file){
    States::StateDiagram<ScreenData>* testG = new States::StateDiagram<ScreenData>();
    try{
        testG->deserializeFromFile(file);
    }
    catch(States::BadXMLException e){
        return;
    }
    delete testG;
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
    ScreenData sd = s->getData();
    sd.setColor(pick);
    s->setData(sd);
    choosedState = s;
    emit nameChng(sd.getName());
    emit infoChng(sd.getInfo());
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
    if(creatingConn || removeCon){
        States::State<ScreenData> *st = findState(x,y);
        if(!stage2){
            unpickState(choosedState);
            pickState(st);
            stage2 = true;
        }
        else
        {
            if(creatingConn)
                graph->addConnection(choosedState,st);
            else
                graph->removeConnection(choosedState,st);
            stage2 = false;
            creatingConn = false;
            removeCon = false;
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
void PaintWidget::drawConnection(States::State<ScreenData>* s1, States::State<ScreenData>* s2 , QPainter &p){
    p.setPen(unpick);
    p.drawLine(findPoint(s1,s2),findPoint(s2,s1));
}

void PaintWidget::drawState(States::State<ScreenData>* s, QPainter &p){


    ScreenData d = s->getData();
    QString str = d.getName();
    int x = d.getX() - offsetX, y = d.getY() - offsetY;
     p.setPen(d.getColor());
    if(graph->isInitialState(s)){
        p.drawEllipse(x+15,y+5,70,70);
        return;
    }
    if(graph->isFinalState(s)){
        p.drawEllipse(x+15,y+5,70,70);
        p.drawEllipse(x,y,30,30);
        return;
    }
    States::SuperState<ScreenData>* test = dynamic_cast<States::SuperState<ScreenData>*>(s);
    if(test != 0){
        p.drawRect(x,y,100,80);
        p.drawRect(x,y,100,15);
        p.drawRect(x+20,y+30,60,50);
        p.setPen(QColor(255,255,255));
        p.drawText(QPoint(x+5,y+14),str);
        return;
    }
    p.drawRect(x,y,100,80);
    p.drawRect(x,y,100,15);
    p.setPen(QColor(255,255,255));
    p.drawText(QPoint(x+5,y+14),str);


}

void PaintWidget::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.fillRect(0,0,width(),height(),QBrush(QColor(0,0,0),Qt::SolidPattern));
    for(States::StateDiagram<ScreenData>::iterator i = graph->begin(); !i.isEnd() ; i++){
        drawState(*i,p);
        for(States::StateDiagram<ScreenData>::iterator j = graph->getConnections(*i); !j.isEnd(); ++j){
            drawConnection(*i,*j, p);
        }
    }
}

void PaintWidget::setGraph(States::StateDiagram<ScreenData>* d){
    graph = d;
}
