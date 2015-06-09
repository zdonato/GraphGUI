#include "graphnode.h"

GraphNode::GraphNode(qreal x, qreal y, qreal _r)
:
  QGraphicsEllipseItem(x - _r, y - _r, _r + _r, _r + _r),
  label(""), cx(x), cy(y), r(_r)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
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

void GraphNode::setLabel(QString _label)
{
    label = _label;
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
}

void GraphNode::moveTo(const QPointF& pt)
{
    cx = pt.x();
    cy = pt.y();

    QRectF newRect = rect();
    newRect.moveLeft(cx - r);
    newRect.moveTop(cy - r);
    setRect(newRect);

    QListIterator<GraphEdge*> it(sourceEdges);
    while (it.hasNext())
        it.next()->sourceUpdated();

    it = QListIterator<GraphEdge*>(destinationEdges);
    while (it.hasNext()) {
        GraphEdge* edge = it.next();
        // The undirected edges were already updated because the node is both source and destination
        if (!edge->isUndirected())
            edge->destinationUpdated();
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
