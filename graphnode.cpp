#include "graphnode.h"

#include <QGraphicsScene>

GraphNode::GraphNode(unsigned _id, qreal x, qreal y, qreal _r)
:
  QGraphicsEllipseItem(x - _r, y - _r, _r + _r, _r + _r),
  id(_id), label(QString::number(_id)), subscript(""), cx(x), cy(y), r(_r)
{

}

GraphNode::~GraphNode()
{

}

unsigned GraphNode::getId()
{
    return id;
}

QPointF GraphNode::getCenter()
{
    return QPointF(cx, cy);
}

qreal GraphNode::getRadius()
{
    return r;
}

QString GraphNode::getLabel()
{
    return label;
}

QString GraphNode::getSubscript()
{
    return subscript;
}

void GraphNode::setCenter(const QPointF& pt)
{
    cx = pt.x();
    cy = pt.y();

    QRectF newRect = rect();
    newRect.moveLeft(cx - r);
    newRect.moveTop(cy - r);
    setRect(newRect);
}

void GraphNode::setLabel(QString _label)
{
    label = _label;
}

void GraphNode::setSubscript(QString s)
{
    subscript = s;
}

void GraphNode::setRadius(qreal _r)
{
    r = _r;
    qreal d = r + r;
    setRect(cx - r, cy - r, d, d);
    update();
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QPen pen(Qt::black, 2);
    QBrush brush(Qt::red);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect());
    painter->drawText(rect(), Qt::AlignCenter, label);

    if (!subscript.isEmpty()) {
        QRectF subRect;
        subRect.setRect(cx - 1.65*r, cy + 1.15*r, 15, 15);
        painter->drawText(subRect, Qt::AlignCenter, subscript);
        scene()->update(subRect);
    }
}

void GraphNode::moveTo(const QPointF& pt)
{
    setCenter(pt);

    QListIterator<GraphEdge*> it(destinationEdges);
    while (it.hasNext())
        it.next()->destinationUpdated();

    it = QListIterator<GraphEdge*>(sourceEdges);
    while (it.hasNext()) {
        GraphEdge* edge = it.next();
        // The undirected edges were already updated because the node is both source and destination
        if (!edge->isUndirected())
            edge->sourceUpdated();
    }
}

QList<GraphEdge*>& GraphNode::getSourceEdges()
{
    return sourceEdges;
}

QList<GraphEdge*>& GraphNode::getDestinationEdges()
{
    return destinationEdges;
}

void GraphNode::addSourceEdge(GraphEdge* e)
{
    sourceEdges.append(e);
}

void GraphNode::addDestinationEdge(GraphEdge* e)
{
    destinationEdges.append(e);
}

void GraphNode::removeSourceEdge(GraphEdge* e)
{
    sourceEdges.removeOne(e);
}

void GraphNode::removeDestinationEdge(GraphEdge* e)
{
    destinationEdges.removeOne(e);
}
