#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QSpinBox>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "parser.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel* radiusLabel = new QLabel("Vertex radius:", ui->toolBar);
    radiusEdit = new QSpinBox(ui->toolBar);
    radiusEdit->setRange(8, 50);
    radiusEdit->stepBy(1);
    radiusEdit->setValue(15);
    radiusEdit->setEnabled(false);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(radiusLabel);
    ui->toolBar->addWidget(radiusEdit);

    status = new QLabel();
    ui->statusBar->addWidget(status);
    ui->graphicsView->setStatus(status);

    connect(ui->menuBar, SIGNAL(triggered(QAction*)), this, SLOT(menuOptionClicked(QAction*)));
    connect(ui->toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolBarOptionClicked(QAction*)));
    connect(radiusEdit, SIGNAL(valueChanged(int)), this, SLOT(radiusChanged(int)));

    setWindowTitle("Graph Algorithms");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete radiusEdit;
    delete status;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QSize size = event->size();
    size.setHeight(size.height() - 68); // consider the size of toolBar and statusBar
    ui->graphicsView->resize(size);
}

void MainWindow::menuOptionClicked(QAction* action)
{
    if (action == ui->actionNew_Unweighted_Directed_Graph) {
        newGraph(false, false);
    }
    else if (action == ui->actionNew_Unweighted_Undirected_Graph) {
        newGraph(false, true);
    }
    if (action == ui->actionNew_Weighted_Directed_Graph) {
        newGraph(true, false);
    }
    else if (action == ui->actionNew_Weighted_Undirected_Graph) {
        newGraph(true, true);
    }
    else if (action == ui->actionOpen) {
        openGraph();
    }
    else if (action == ui->actionSave) {
        saveGraph();
    }
    else if (action == ui->actionSave_as) {
        saveGraphAs();
    }
    else if (action == ui->actionPNG) {
        QString filePath = QFileDialog::getSaveFileName(this, "Export to PNG", QCoreApplication::applicationDirPath(), "PNG (*.png)");

        if (!filePath.isNull()) {
            QPixmap pixMap = ui->graphicsView->grab();
            pixMap.save(filePath);
        }
    }
    else if (action == ui->actionClose) {
        closeGraph();
    }
    else if (action == ui->actionQuit) {
        close();
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
    else if (action == ui->actionMove)
        ui->graphicsView->setCurrentAction(MOVING);
}

void MainWindow::radiusChanged(int r)
{
    ui->graphicsView->setNodeRadius(r);
}

void MainWindow::newGraph(bool weighted, bool undirected)
{
//    if (fileOpen)
//        closeGraph();

    ui->graphicsView->setGraphWeighted(weighted);
    ui->graphicsView->setGraphUndirected(undirected);

    ui->graphicsView->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionPNG->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionMove->setEnabled(true);
    ui->actionAdd_Vertex->setEnabled(true);
    ui->actionAdd_Undirected_Edge->setEnabled(true);
    radiusEdit->setEnabled(true);

    if (!undirected)
        ui->actionAdd_Directed_Edge->setEnabled(true);

    setWindowTitle("Graph Algorithms - New");
    ui->graphicsView->updateStatus();
}

void MainWindow::closeGraph()
{
//    if (!graphAlreadySavedBefore) {
//        if (userWantsToSave)
//            saveGraph();
//        else
           ui->graphicsView->clear();
//    }

    ui->graphicsView->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
    ui->actionPNG->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionMove->setEnabled(false);
    ui->actionAdd_Vertex->setEnabled(false);
    ui->actionAdd_Undirected_Edge->setEnabled(false);
    ui->actionAdd_Directed_Edge->setEnabled(false);
    radiusEdit->setEnabled(false);

    setWindowTitle("Graph Algorithms");
}

void MainWindow::openGraph()
{
//    if (fileOpen)
        closeGraph();

    QString filePath = QFileDialog::getOpenFileName(this, "Open Graph", QCoreApplication::applicationDirPath(), "Graph Algorithms File (*.gaf)");
    QFileInfo fileInfo(filePath);

    Parser parser;
    parser.parseGraph(filePath);

    QListIterator<GraphNode*> it(parser.getNodes());
    while (it.hasNext()) {
        GraphNode* node = it.next();

        ui->graphicsView->scene()->addItem(node);

        QListIterator<GraphEdge*> edgesIt(node->getSourceEdges());
        while (edgesIt.hasNext()) {
            GraphEdge* edge = edgesIt.next();

            ui->graphicsView->scene()->addItem(edge);
        }

        edgesIt = QListIterator<GraphEdge*>(node->getSourceEdges());
        while (edgesIt.hasNext()) {
            GraphEdge* edge = edgesIt.next();

            if (!edge->isUndirected())
                ui->graphicsView->scene()->addItem(edge);
        }
    }

    ui->graphicsView->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionPNG->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionMove->setEnabled(true);
    ui->actionAdd_Vertex->setEnabled(true);
    ui->actionAdd_Undirected_Edge->setEnabled(true);
    radiusEdit->setEnabled(true);

    setWindowTitle("Graph Algorithms - " + fileInfo.baseName());
}

void MainWindow::saveGraph()
{
//    if (graphAlreadySavedBefore)
//        save graph to file here
//    else
//        saveGraphAs();
}

void MainWindow::saveGraphAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Graph", QCoreApplication::applicationDirPath(), "Graph Algorithms File (*.gaf)");

    if (!filePath.isNull()) {
        saveGraph();
        QFileInfo fileInfo(filePath);
        setWindowTitle("Graph Algorithms - " + fileInfo.fileName());
    }
}

