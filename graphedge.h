#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsLineItem>
#include <QPainter>

class GraphEdge : public QGraphicsLineItem
{
public:
    GraphEdge(qreal x1, qreal y1, qreal x2, qreal y2);
    virtual ~GraphEdge(){}

    void sourceUpdated(const QPointF& pt);
    void destinationUpdated(const QPointF& pt);

private:
    qreal x1, y1, x2, y2;
};

#endif // GRAPHEDGE_H
