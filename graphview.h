#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graphnode.h"

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = 0);
    virtual ~GraphView();

private:
    QGraphicsScene* scene;
    QGraphicsItem* selectedItem;
    GraphNode* edgeSource;

protected slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GRAPHVIEW_H
