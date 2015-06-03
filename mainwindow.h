#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "graphview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

protected slots:
    void menuOptionClicked(QAction* action);
    void toolBarOptionClicked(QAction* action);
    void radiusChanged(int r);
    void resizeEvent(QResizeEvent *);

};

#endif // MAINWINDOW_H
