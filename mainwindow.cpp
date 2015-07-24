#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QMessageBox>
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

    undoStack = new QUndoStack();
    ui->graphicsView->setUndoStack(undoStack);
    undoAction = undoStack->createUndoAction(this, "&Undo");
    undoAction->setShortcuts(QKeySequence::Undo);
    ui->menuEdit->addAction(undoAction);
    redoAction = undoStack->createRedoAction(this, "&Redo");
    redoAction->setShortcuts(QKeySequence::Redo);
    ui->menuEdit->addAction(redoAction);

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        ui->graphicsView->setCurrentAction(NONE);
}

void MainWindow::menuOptionClicked(QAction* action)
{
    ui->graphicsView->setCurrentAction(NONE);

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
        if (ui->graphicsView->getFilePath().isEmpty())
            saveGraphAs();
        else {
            QFileInfo fileInfo(ui->graphicsView->getFilePath());
            saveGraph(fileInfo);
        }
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
    if (ui->graphicsView->getFilePath().isEmpty() || closeGraph()) {
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
        ui->graphicsView->setFilePath("");
        ui->graphicsView->updateStatus();
    }
}

bool MainWindow::closeGraph()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save Changes", "Would you like to save your changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        QFileInfo fileInfo(ui->graphicsView->getFilePath());
        saveGraph(fileInfo);
    }

    bool closed = reply != QMessageBox::Cancel;

    if (closed) {
        ui->graphicsView->clear();

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

    return closed;
}

void MainWindow::openGraph()
{
    if (ui->graphicsView->getFilePath().isEmpty() || closeGraph()) {
        QString filePath = QFileDialog::getOpenFileName(this, "Open Graph", QCoreApplication::applicationDirPath(), "Graph Algorithms File (*.gaf)");

        if (!filePath.isNull()) {
            QFileInfo fileInfo(filePath);

            Parser parser;
            parser.parseGraph(filePath);
            ui->graphicsView->setFilePath(filePath);
            ui->graphicsView->setGraphUndirected(parser.isGraphUndirected());
            ui->graphicsView->setGraphWeighted(parser.isGraphWeighted());

            QListIterator<GraphNode*> it(parser.getNodes());
            while (it.hasNext()) {
                GraphNode* node = it.next();
                ui->graphicsView->addNode(node);
            }

            QListIterator<GraphEdge*> itE(parser.getEdges());
            while (itE.hasNext()) {
                GraphEdge* edge = itE.next();
                ui->graphicsView->addEdge(edge);
            }

            ui->graphicsView->setEnabled(true);
            ui->actionSave->setEnabled(true);
            ui->actionSave_as->setEnabled(true);
            ui->actionPNG->setEnabled(true);
            ui->actionClose->setEnabled(true);
            ui->actionMove->setEnabled(true);
            ui->actionAdd_Vertex->setEnabled(true);
            ui->actionAdd_Undirected_Edge->setEnabled(true);
            ui->actionAdd_Directed_Edge->setEnabled(!parser.isGraphUndirected());
            radiusEdit->setEnabled(true);

            setWindowTitle("Graph Algorithms - " + fileInfo.fileName());
        }
    }
}

void MainWindow::saveGraph(const QFileInfo& fileInfo)
{
    Parser parser;
    parser.setNodes(ui->graphicsView->getNodes());
    parser.setEdges(ui->graphicsView->getEdges());
    parser.setGraphWeight(ui->graphicsView->isGraphWeighted());
    parser.setEdgeType(ui->graphicsView->isGraphUndirected());
    parser.setNodeRadius(ui->graphicsView->getNodeRadius());

    parser.saveGraph(fileInfo.absoluteFilePath());
    ui->graphicsView->setFilePath(fileInfo.absoluteFilePath());
}

void MainWindow::saveGraphAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Graph", QCoreApplication::applicationDirPath(), "Graph Algorithms File (*.gaf)");

    if (!filePath.isNull()) {
        QFileInfo fileInfo(filePath);
        saveGraph(fileInfo);
        setWindowTitle("Graph Algorithms - " + fileInfo.fileName());
    }
}

