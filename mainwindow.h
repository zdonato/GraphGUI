#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QUndoStack>
#include <QFileInfo>

#include "algorithmslibrary.h"
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
    QSpinBox* radiusEdit;
    QLabel* status;
    QUndoStack* undoStack;
    QAction* undoAction;
    QAction* redoAction;
    AlgorithmsLibrary algorithmsLibrary;

    void newGraph(bool weighted, bool undirected);
    bool closeGraph();
    void openGraph();
    void saveGraph(const QFileInfo& fileInfo);
    void saveGraphAs();
    void loadLibrary();

  protected slots:
    void menuOptionClicked(QAction* action);
    void toolBarOptionClicked(QAction* action);
    void radiusChanged(int r);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
