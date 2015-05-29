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

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void sourceUpdated();
    void destinationUpdated();

private:
    void createDrawing();

    GraphNode* source;
    GraphNode* destination;
    QGraphicsLineItem* mainLine;
    QGraphicsLineItem* destinationArrowLine1;
    QGraphicsLineItem* destinationArrowLine2;
    QGraphicsLineItem* sourceArrowLine1;
    QGraphicsLineItem* sourceArrowLine2;
    QPainterPath* shapePath;
    bool undirected;
};

#endif // GRAPHEDGE_H
