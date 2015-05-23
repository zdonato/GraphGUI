#include "graphedge.h"

GraphEdge::GraphEdge(qreal _x1, qreal _y1, qreal _x2, qreal _y2)
:
  QGraphicsLineItem(_x1, _y1, _x2, _y2),
  x1(_x1), y1(_y1), x2(_x2), y2(_y2)
{
}

void GraphEdge::sourceUpdated(const QPointF& pt)
{
    x1 = pt.x();
    y1 = pt.y();
    setLine(x1, y1, x2, y2);
    update();
}

void GraphEdge::destinationUpdated(const QPointF& pt)
{
    x2 = pt.x();
    y2 = pt.y();
    setLine(x1, y1, x2, y2);
    update();
}
