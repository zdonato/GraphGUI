#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel* radiusLabel = new QLabel("Vertex radius:", ui->toolBar);
    QSpinBox* radiusEdit = new QSpinBox(ui->toolBar);
    radiusEdit->setRange(8, 50);
    radiusEdit->stepBy(1);
    radiusEdit->setValue(15);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(radiusLabel);
    ui->toolBar->addWidget(radiusEdit);

    connect(ui->menuBar, SIGNAL(triggered(QAction*)), this, SLOT(menuOptionClicked(QAction*)));
    connect(ui->toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolBarOptionClicked(QAction*)));
    connect(radiusEdit, SIGNAL(valueChanged(int)), this, SLOT(radiusChanged(int)));

    setWindowTitle("Graph Algorithms");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::menuOptionClicked(QAction* action)
{
    if (action == ui->actionUnweighted_graph) {

    }
    else if (action == ui->actionWeighted_Graph) {

    }
    else if (action == ui->actionOpen) {

    }
    else if (action == ui->actionSave) {

    }
    else if (action == ui->actionSave_as) {

    }
    else if (action == ui->actionPNG) {
        QString filePath = QFileDialog::getSaveFileName(this, "Export to PNG", QCoreApplication::applicationDirPath(), "PNG (*.png)");

        if (!filePath.isNull()) {
            QPixmap pixMap = ui->graphicsView->grab();
            pixMap.save(filePath);
        }
    }
    else if (action == ui->actionClose) {

    }
    else if (action == ui->actionQuit) {

    }
}

void MainWindow::toolBarOptionClicked(QAction* action)
{
    if (action == ui->actionAdd_Vertex)
        ui->graphicsView->setCurrentAction(ADD_VERTEX);
    else if (action == ui->actionAdd_Directed_Edge)
        ui->graphicsView->setCurrentAction(ADD_DIRECTED_EDGE);
    else if (action == ui->actionAdd_Undirected_Edge)
        ui->graphicsView->setCurrentAction(ADD_UNDIRECTED_EDGE);
}

void MainWindow::radiusChanged(int r)
{
    ui->graphicsView->setNodeRadius(r);
}
