#include "graphnode.h"

GraphNode::GraphNode(qreal x, qreal y, qreal _r)
:
  QGraphicsEllipseItem(x - _r, y - _r, _r + _r, _r + _r),
  cx(x), cy(y), r(_r), sourceEdge(NULL), destinationEdge(NULL)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
}

QPointF GraphNode::getCenter()
{
    return QPointF(cx, cy);
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QPen pen(Qt::black, 2);
    QBrush brush(Qt::red);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect());
    painter->drawText(rect(), Qt::AlignCenter, "x");
}

void GraphNode::moveTo(const QPointF& pt)
{
    cx = pt.x();
    cy = pt.y();

    QRectF newRect = rect();
    newRect.moveLeft(cx - r);
    newRect.moveTop(cy - r);
    setRect(newRect);
    update();

    if (sourceEdge)
        sourceEdge->sourceUpdated(pt);
    if (destinationEdge)
        destinationEdge->destinationUpdated(pt);
}

void GraphNode::addSourceEdge(GraphEdge* e)
{
    sourceEdge = e;
}

void GraphNode::addDestinationEdge(GraphEdge* e)
{
    destinationEdge = e;
}
