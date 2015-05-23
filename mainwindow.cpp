#include <QtGui>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    graphView = new GraphView();
//    graphView->setGeometry(0, 0, 50, 50);

//    graphView->setMaximumSize(50,300);
//    graphView->setMinimumSize(50,300);
//    graphView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    drawButton = new QPushButton("Draw Circle", this);

//    QVBoxLayout *mainLayout = new QVBoxLayout();
//    mainLayout->addWidget(graphView);
//    mainLayout->addWidget(drawButton, 0, Qt::AlignHCenter);
//    setLayout(mainLayout);

    ui->setupUi(this);

    setWindowTitle(tr("Basic Drawing"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
