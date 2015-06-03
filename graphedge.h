#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QPainterPath>

class GraphNode;

class GraphEdge : public QGraphicsItem
{
public:
    GraphEdge(GraphNode* source, GraphNode* destination, bool undirected);
    virtual ~GraphEdge();

    void updateDrawing();

    int getWeight();
    void setWeight(int w);

    bool isUndirected();
    void setUndirected(bool undirected);

    GraphNode* getSourceNode();
    GraphNode* getDestinationNode();

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void sourceUpdated();
    void destinationUpdated();
    void changeDirection();

private:    
    GraphNode* source;
    GraphNode* destination;
    QGraphicsLineItem* mainLine;
    QGraphicsLineItem* destinationArrowLine1;
    QGraphicsLineItem* destinationArrowLine2;
    QGraphicsLineItem* sourceArrowLine1;
    QGraphicsLineItem* sourceArrowLine2;
    QGraphicsTextItem* weightText;
    QPainterPath* shapePath;
    bool undirected;
    int weight;
};

#endif // GRAPHEDGE_H
