#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QUndoStack>
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

    int getNodeRadius();
    void setNodeRadius(int radius);

    bool isGraphWeighted();
    void setGraphWeighted(bool weighted);

    bool isGraphUndirected();
    void setGraphUndirected(bool undirected);

    void setUndoStack(QUndoStack* stack);
    void addNode(GraphNode* node);
    void addEdge(GraphEdge* edge);
    void removeNode(GraphNode* node);
    void removeEdge(GraphEdge* edge);
    QList<GraphNode*> getNodes();
    QList<GraphEdge*> getEdges();

  private:
    void executeContextMenu(const QPoint& menuPosition);
    void removeItemCommand(GraphNode* node);
    void removeItemCommand(GraphEdge* edge);
    void changeLabel(GraphNode* node);
    void changeWeight(GraphEdge* edge);
    void addEdgeCommand(bool undirected);
    void addNodeCommand(const QPointF& pt);
    QString actionString(GraphAction action);

    QGraphicsItem* selectedItem;
    GraphNode* edgeSource;
    GraphEdge* currentEdge;
    GraphNode* currentNode;
    GraphAction currentAction;
    QLabel* status;
    QUndoStack* undoStack;
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
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // GRAPHVIEW_H
