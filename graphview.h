#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graphedge.h"
#include "graphnode.h"

enum GraphAction {
    NONE,
    ADD_VERTEX,
    ADD_DIRECTED_EDGE,
    ADD_UNDIRECTED_EDGE
};

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = 0);
    virtual ~GraphView();

    void setCurrentAction(GraphAction action);
    void setNodeRadius(int radius);

private:
    void executeContextMenu(const QPoint& menuPosition);
    void removeItem(GraphNode* node);
    void removeItem(GraphEdge* edge);
    void changeLabel(GraphNode* node);
    void changeWeight(GraphEdge* edge);

    QGraphicsItem* selectedItem;
    GraphNode* edgeSource;
    GraphAction currentAction;
    int nodeRadius;

protected slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GRAPHVIEW_H
