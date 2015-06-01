#include "mainwindow.h"
#include <QApplication>
#include <testclass.h>
#include <simplelogger.h>
#include <vector>
#include <StateGraph.h>
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




    w.show();

    return a.exec();
}
