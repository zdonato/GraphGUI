#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "graphedge.h"

class GraphNode : public QGraphicsEllipseItem
{
public:
    GraphNode(qreal x, qreal y, qreal r);
    virtual ~GraphNode(){}

    QPointF getCenter();
    qreal getRadius();
    QString getLabel();
    void setLabel(QString label);
    void setRadius(qreal r);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void moveTo(const QPointF& pt);
    void addSourceEdge(GraphEdge* e);
    void addDestinationEdge(GraphEdge* e);

private:
    QString label;
    qreal cx, cy, r;
    GraphEdge* sourceEdge;
    GraphEdge* destinationEdge;

};

#endif // GRAPHNODE_H
