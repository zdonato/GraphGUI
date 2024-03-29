#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#include "graphedge.h"

class GraphNode : public QGraphicsEllipseItem
{
  public:
    GraphNode(unsigned id, qreal x, qreal y, qreal r);
    virtual ~GraphNode();

    unsigned getId();
    QPointF getCenter();
    qreal getRadius();
    QString getLabel();
    QString getSubscript();
    void setCenter(const QPointF& p);
    void setLabel(QString label);
    void setSubscript(QString s);
    void setRadius(qreal r);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void moveTo(const QPointF& pt);

    QList<GraphEdge*>& getSourceEdges();
    QList<GraphEdge*>& getDestinationEdges();
    void addSourceEdge(GraphEdge* e);
    void addDestinationEdge(GraphEdge* e);
    void removeSourceEdge(GraphEdge* e);
    void removeDestinationEdge(GraphEdge* e);

  private:
    unsigned id;
    QString label;
    QString subscript;
    qreal cx, cy, r;
    QList<GraphEdge*> sourceEdges;
    QList<GraphEdge*> destinationEdges;

};

#endif // GRAPHNODE_H
