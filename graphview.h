#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include "graphedge.h"
#include "graphnode.h"

enum GraphAction {
    NONE,
    MOVING,
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

    void setStatus(QLabel* status);
    void updateStatus();
    void clear();
    void setCurrentAction(GraphAction action);
    void setNodeRadius(int radius);
    void setGraphWeighted(bool weighted);
    void setGraphUndirected(bool undirected);

private:
    void executeContextMenu(const QPoint& menuPosition);
    void removeItem(GraphNode* node);
    void removeItem(GraphEdge* edge);
    void changeLabel(GraphNode* node);
    void changeWeight(GraphEdge* edge);
    void addEdge(bool undirected);
    QString actionString(GraphAction action);

    QGraphicsItem* selectedItem;
    GraphNode* edgeSource;
    GraphAction currentAction;
    QLabel* status;
    int nodeRadius;
    bool weightedGraph;
    bool undirectedGraph;
    unsigned numberOfNodes;
    unsigned numberOfEdges;
    unsigned nextNodeID;

protected slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // GRAPHVIEW_H
