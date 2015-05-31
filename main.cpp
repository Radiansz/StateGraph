#include "mainwindow.h"
#include <QApplication>
#include <testclass.h>
#include <simplelogger.h>
#include <vector>
#include <stategraph.h>
#include <state.h>
#include <superstate.h>
#include <iostream>

#include <stateexception.h>
#include <unknownstateexception.h>
#include <wrongindexstateexception.h>
#include <iostream>
#include <QMessageBox>

void exceptionHandler(){
    States::StateException* e = States::StateException::getLastException();
    QMessageBox::critical(NULL,"Error",e->what(),0,0);
    exit(3);
}

int main(int argc, char *argv[])
{
    std::istream inp();
    std::set_terminate(exceptionHandler);
    QApplication a(argc, argv);
    MainWindow w;
    Logger::write("Hello world");
   // throw States::WrongIndexStateException();
    testClass test("Hello", 3);
    QString res = test.serialize();
    Logger::write(res);
    States::StateGraph<testClass> graph;
    States::StateGraph<testClass> graph2;
    graph.deserializeFromFile("Graph.txt");
    std::cout << graph << graph.statesDesc(1);
/*    States::State<testClass>* gs1 = new States::State<testClass>(testClass("SubNode",1));
    States::State<testClass>* gs2 = new States::State<testClass>(testClass("SubNode",2));
    States::State<testClass>* gs3 = new States::State<testClass>(testClass("SubNode",3));
    graph2.addState(gs1);
    graph2.addState(gs2);
    graph2.addState(gs3);
    graph2.addConnection(0,1);
    graph2.addConnection(1,2);
    graph2.setInitialState(gs1);
    graph2.setFinalState(gs3);

    testClass rlyTemp("Testing this!",9);
    States::State<testClass> ha(rlyTemp);
    res = ha.serialize();
    Logger::write(res);
    States::SuperState<testClass>* s1 = new States::SuperState<testClass>(testClass("Node",1));
    s1->setInnerGraph(&graph2);
    States::State<testClass> s2(testClass("Node",2));
    States::State<testClass> s3(testClass("Node",3));
    States::State<testClass> s4(testClass("Node",4));
    States::State<testClass> s5(testClass("Node",5));
    States::State<testClass> *st;
    States::SuperState<int> *sst;
    graph.addState(s1);
    graph.addState(&s2);
    graph.addState(&s3);
    graph.addState(&s4);
    graph.addState(&s5);
    graph.addConnection(0,1);
    graph.addConnection(1,2);
    graph.addConnection(1,3);
    graph.addConnection(3,4);
    graph.setInitialState(s1);
    graph.setFinalState(&s5);
    graph.findPath(s1,&s5);
    res = graph.serialize();*/
    graph.serializeToFile("newGraph.sg");
    Logger::write(res);



    w.show();

    return a.exec();
}
