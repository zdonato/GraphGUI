#include <QLineF>

#include "graphedge.h"
#include "graphnode.h"

GraphEdge::GraphEdge(GraphNode* src, GraphNode* dst, bool _undirected, bool _drawArrows, bool _drawWeight, bool _drawAsArc)
:
  QGraphicsItem(),
  source(src),
  destination(dst),
  destinationCenter(dst->getCenter()),
  shapePath(NULL),
  weightText(NULL),
  undirected(_undirected),
  drawArrows(_drawArrows),
  drawWeight(_drawWeight),
  drawAsArc(_drawAsArc),
  weight(1),
  bold(false)
{
    updateDrawing();
}

GraphEdge::GraphEdge(GraphNode* src, const QPointF& endPoint, bool _undirected, bool _drawArrows, bool _drawWeight, bool _drawAsArc)
:
  QGraphicsItem(),
  source(src),
  destination(NULL),
  destinationCenter(endPoint),
  shapePath(NULL),
  weightText(NULL),
  undirected(_undirected),
  drawArrows(_drawArrows),
  drawWeight(_drawWeight),
  drawAsArc(_drawAsArc),
  weight(1),
  bold(false)
{
    updateDrawing();
}

GraphEdge::~GraphEdge()
{
    cleanDrawing();
}

void GraphEdge::cleanDrawing()
{
    delete shapePath;
    delete weightText;

    shapePath = NULL;
    weightText = NULL;
}

void GraphEdge::updateDrawing()
{
    cleanDrawing();

    if (drawAsArc)
        createArcDrawing();
    else
        createStraightLineDrawing();
}

void GraphEdge::createArcDrawing()
{
    QPointF sourceCenter = source->getCenter();
    QLineF line(sourceCenter, destinationCenter);
    qreal dstRadius = destination ? destination->getRadius() : source->getRadius();

    QPointF center = line.pointAt(0.5);
    QPointF curvePoint(center.x(), center.y() + 30);
    QLineF curveLine(center, curvePoint);
    curveLine.setAngle(line.angle() + 90);
    curvePoint = curveLine.p2();

    if (drawWeight) {
        QPointF pt(curvePoint.x() + 5, curvePoint.y());
        QLineF textLine(curvePoint, pt);
        qreal lineAngle = line.angle();
        textLine.setAngle(lineAngle + 90);
        weightText = new QGraphicsTextItem(QString::number(weight));
        weightText->setX(textLine.p2().x());
        weightText->setY(textLine.p2().y());
    }

    QPointF pi(sourceCenter);
    QPointF p0(pi.x(), pi.y() + source->getRadius());
    QLineF radiusLine(pi, p0);
    radiusLine.setAngle(line.angle() + 45);
    pi = radiusLine.p2();
    line.setP1(pi);

    QPointF pf(destinationCenter);
    p0 = QPointF(pf.x(), pf.y() + dstRadius);
    radiusLine = QLineF(pf, p0);
    radiusLine.setAngle(line.angle() + 135);
    pf = radiusLine.p2();
    line.setP1(pf);

    shapePath = new QPainterPath();
    shapePath->moveTo(pi);
    shapePath->quadTo(curvePoint, pf);

    if (drawArrows) {
        QPointF p1(pf);
        QPointF p2(p1.x() + 13, p1.y());
        QLineF arrowLine1(p1, p2);
        arrowLine1.setAngle(line.angle() - 105);
        QLineF arrowLine2(p1, p2);
        arrowLine2.setAngle(line.angle() + 155);

        shapePath->moveTo(arrowLine1.p2());
        shapePath->lineTo(pf);
        shapePath->lineTo(arrowLine2.p2());

        if (undirected) {
            QPointF p3(pi);
            QPointF p4(p3.x() + 13, p3.y());
            QLineF arrowLine3(p3, p4);
            arrowLine3.setAngle(line.angle() + 120);
            QLineF arrowLine4(p3, p4);
            arrowLine4.setAngle(line.angle() + 20);

            shapePath->moveTo(arrowLine3.p2());
            shapePath->lineTo(pi);
            shapePath->lineTo(arrowLine4.p2());
        }
    }
}

void GraphEdge::createStraightLineDrawing()
{    
    QPointF sourceCenter = source->getCenter();
    QLineF line(sourceCenter, destinationCenter);
    qreal dstRadius = destination ? destination->getRadius() : source->getRadius();

    QPointF pi(sourceCenter);
    QPointF p0(pi.x() + source->getRadius(), pi.y());
    QLineF radiusLine(pi, p0);
    radiusLine.setAngle(line.angle());
    pi = radiusLine.p2();
    line.setP1(pi);

    QPointF pf(destinationCenter);
    p0 = QPointF(pf.x() + dstRadius, pf.y());
    radiusLine = QLineF(pf, p0);
    radiusLine.setAngle(line.angle() - 180);
    pf = radiusLine.p2();
    line.setP2(pf);

    shapePath = new QPainterPath();
    shapePath->moveTo(pi);
    shapePath->lineTo(pf);

    if (drawArrows) {
        QPointF p1(pf);
        QPointF p2(p1.x() + 13, p1.y());
        QLineF arrowLine1(p1, p2);
        arrowLine1.setAngle(line.angle() - 135);
        QLineF arrowLine2(p1, p2);
        arrowLine2.setAngle(line.angle() + 135);

        shapePath->moveTo(arrowLine1.p2());
        shapePath->lineTo(pf);
        shapePath->lineTo(arrowLine2.p2());

        if (undirected) {
            QPointF p3(pi);
            QPointF p4(p3.x() + 13, p3.y());
            QLineF arrowLine3(p3, p4);
            arrowLine3.setAngle(line.angle() + 45);
            QLineF arrowLine4(p3, p4);
            arrowLine4.setAngle(line.angle() - 45);

            shapePath->moveTo(arrowLine3.p2());
            shapePath->lineTo(pi);
            shapePath->lineTo(arrowLine4.p2());
        }
    }

    if (drawWeight) {
        p0 = line.pointAt(0.5);
        QPointF pt(p0.x() + 13, p0.y());
        QLineF textLine(p0, pt);
        qreal lineAngle = line.angle();
        textLine.setAngle(lineAngle + 90);
        weightText = new QGraphicsTextItem(QString::number(weight));
        weightText->setX(textLine.p2().x());
        weightText->setY(textLine.p2().y());
    }
}

int GraphEdge::getWeight()
{
    return weight;
}

void GraphEdge::setWeight(int w)
{
    weight = w;
    if (weightText)
        weightText->setPlainText(QString::number(weight));
}

bool GraphEdge::isUndirected()
{
    return undirected;
}

void GraphEdge::setUndirected(bool _undirected)
{
    if (_undirected != undirected) {        
        undirected = _undirected;

        if (undirected) {
            source->addDestinationEdge(this);
            destination->addSourceEdge(this);
        }
        else {
            source->removeDestinationEdge(this);
            destination->removeSourceEdge(this);
        }

        updateDrawing();
    }
}

void GraphEdge::setBold(bool _bold)
{
    bold = _bold;
}

void GraphEdge::setDrawArrows(bool draw)
{
    if (draw != drawArrows) {
        drawArrows = draw;
        updateDrawing();
    }
}

void GraphEdge::setDrawWeight(bool draw)
{
    if (draw != drawWeight) {
        drawWeight = draw;
        updateDrawing();
    }
}

void GraphEdge::setDrawAsArc(bool draw)
{
    if (draw != drawAsArc) {
        drawAsArc = draw;
        updateDrawing();
    }
}

GraphNode* GraphEdge::getSourceNode()
{
    return source;
}

GraphNode* GraphEdge::getDestinationNode()
{
    return destination;
}

void GraphEdge::setDestinationNode(GraphNode* node)
{
    destination = node;
    destinationCenter = node->getCenter();
    updateDrawing();
}

void GraphEdge::setEndPoint(const QPointF& endPoint)
{
    destinationCenter = endPoint;
    updateDrawing();
}

QRectF GraphEdge::boundingRect() const
{
    return shapePath->boundingRect();
}

QPainterPath GraphEdge::shape() const
{
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    return stroker.createStroke(*shapePath);
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    QPen pen(Qt::black, bold ? 3 : 1);
    painter->setPen(pen);
    painter->drawPath(*shapePath);

    if (drawWeight) {
        painter->save();
        painter->translate(weightText->pos().toPoint());
        painter->drawText(0 - 0.35*weightText->boundingRect().width(), 0 + 0.2*weightText->boundingRect().height(), QString::number(weight));
        painter->restore();
    }
}

void GraphEdge::sourceUpdated()
{
    updateDrawing();
}

void GraphEdge::destinationUpdated()
{
    destinationCenter = destination->getCenter();
    updateDrawing();
}

void GraphEdge::changeDirection()
{
    source->removeSourceEdge(this);
    destination->removeDestinationEdge(this);

    GraphNode* temp = source;
    source = destination;
    destination = temp;

    source->addSourceEdge(this);
    destination->addDestinationEdge(this);

    updateDrawing();
}
