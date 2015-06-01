#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(QWidget *parent, States::StateDiagram<ScreenData>* graph):MainWindow(parent){
    ui->widget->setGraph(graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Open file"), "","");
    emit openFile(file);
}

void MainWindow::on_pushButton_clicked(){

}

void MainWindow::on_actionSave_triggered()
{
    QString file = QFileDialog::getSaveFileName(this,tr("Open file"), "","");
    emit saveFile(file);
}

void MainWindow::on_plainTextEdit_textChanged()
{
    emit infoChng(ui->infoBox->toPlainText());
}
