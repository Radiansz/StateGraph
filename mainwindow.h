#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <stategraph.h>
#include <screendata.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *parent, States::StateDiagram<ScreenData>* graph);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_plainTextEdit_textChanged();

signals:
    void openFile(QString file);
    void saveFile(QString file);
    void infoChng(QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
