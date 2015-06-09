#include <QFileDialog>
#include <QGraphicsView>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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

    connect(ui->menuBar, SIGNAL(triggered(QAction*)), this, SLOT(menuOptionClicked(QAction*)));
    connect(ui->toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolBarOptionClicked(QAction*)));
    connect(radiusEdit, SIGNAL(valueChanged(int)), this, SLOT(radiusChanged(int)));

    setWindowTitle("Graph Algorithms");
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::closeGraph()
{
//    if (!graphAlreadySavedBefore) {
//        if (userWantsToSave)
//            saveGraph();
//        else
           ui->graphicsView->scene()->clear();
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
//        closeGraph();

    QString filePath = QFileDialog::getOpenFileName(this, "Open Graph", QCoreApplication::applicationDirPath(), "Graph Algorithms File (*.gaf)");

    if (!filePath.isNull()) {

        // read graph from file here
        // dont forget to enable the menu functions

        QFileInfo fileInfo(filePath);
        const QString absPath = fileInfo.absoluteFilePath();

        if (!absPath.isNull())
        {
            // Open file.
            std::ifstream file (absPath.toStdString().c_str());

            if (file.is_open())
            {
                std::string content( (std::istreambuf_iterator<char>(file) ),
                                     (std::istreambuf_iterator<char>() ));

                rapidjson::Document d;
                d.Parse(content.c_str());
                rapidjson::Value& val = d["edgeType"];

                //Stringify the json.
//                rapidjson::StringBuffer buffer;
//                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//                d.Accept(writer);

                // Test out each key value pair.
                std::cout << "This graph is: " << val.GetString() << std::endl;

                val = d["graphWeight"];
                std::cout << "This graph is: " << val.GetString() << std::endl;

                val = d["radius"];
                std::cout << "The nodes have radius: " << val.GetInt() << std::endl;

                std::cout << "The nodes of the graph are: " << std::endl;

                const rapidjson::Value& nodesArray = d["nodes"];
                assert(nodesArray.IsArray());
                for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i)
                {
                    std::printf("   Node %s\n", nodesArray[i]["label"].GetString());
                    std::printf("       X: %d\n", nodesArray[i]["x"].GetInt());
                    std::printf("       Y: %d\n", nodesArray[i]["y"].GetInt());
                    std::printf("       ID: %d\n", nodesArray[i]["id"].GetInt());

                    // Iterate over the adjacent nodes.
                    std::cout <<"       Adjacent nodes: " << std::endl;

                    const rapidjson::Value& adjArray = nodesArray[i]["adjacent"];
                    if (adjArray.IsNull() || !adjArray.IsArray())
                    {
                        std::cout << "              No adjacent nodes" << std::endl;
                    }
                    else
                    {
                        for (rapidjson::SizeType j = 0; j < adjArray.Size(); ++j)
                        {
                            std::printf("           Node ID: %d\n", adjArray[j]["nodeID"].GetInt());
                        }
                    }
                }

                file.close();
            }
            else
            {
                std::cout << "Unable to open file: " << fileInfo.fileName().toStdString() << std::endl;
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

