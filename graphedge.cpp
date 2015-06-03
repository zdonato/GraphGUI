#include <QLineF>

#include "graphedge.h"
#include "graphnode.h"

GraphEdge::GraphEdge(GraphNode* src, GraphNode* dst, bool _undirected)
:
  QGraphicsItem(),
  source(src),
  destination(dst),
  mainLine(NULL),
  destinationArrowLine1(NULL),
  destinationArrowLine2(NULL),
  sourceArrowLine1(NULL),
  sourceArrowLine2(NULL),
  weightText(NULL),
  shapePath(NULL),
  undirected(_undirected),
  weight(10)
{
    createDrawing();
}

GraphEdge::~GraphEdge()
{
    delete mainLine;
    delete destinationArrowLine1;
    delete destinationArrowLine2;
    delete sourceArrowLine1;
    delete sourceArrowLine2;
    delete weightText;
    delete shapePath;
}

void GraphEdge::createDrawing()
{
    delete mainLine;
    delete destinationArrowLine1;
    delete destinationArrowLine2;
    delete sourceArrowLine1;
    delete sourceArrowLine2;
    delete weightText;
    delete shapePath;

    shapePath = new QPainterPath();
    QPointF sourceCenter = source->getCenter();
    QPointF destinationCenter = destination->getCenter();

    QLineF line(sourceCenter, destinationCenter);

    QPointF pi(sourceCenter);
    QPointF p0(pi.x() + source->getRadius(), pi.y());
    QLineF radiusLine(pi, p0);
    radiusLine.setAngle(line.angle());
    pi = radiusLine.p2();
    line.setP1(pi);

    QPointF pf(destinationCenter);
    p0 = QPointF(pf.x() + destination->getRadius(), pf.y());
    radiusLine = QLineF(pf, p0);
    radiusLine.setAngle(line.angle() - 180);
    pf = radiusLine.p2();
    line.setP2(pf);

    QPointF p1(pf);
    QPointF p2(p1.x() + 15, p1.y());
    QLineF arrowLine1(p1, p2);
    arrowLine1.setAngle(line.angle() - 135);
    QLineF arrowLine2(p1, p2);
    arrowLine2.setAngle(line.angle() + 135);

    mainLine = new QGraphicsLineItem(line);
    destinationArrowLine1 = new QGraphicsLineItem(arrowLine1);
    destinationArrowLine2 = new QGraphicsLineItem(arrowLine2);

    shapePath->moveTo(arrowLine1.p2());
    shapePath->lineTo(pf);
    shapePath->lineTo(arrowLine2.p2());

    if (undirected) {
        QPointF p3(pi);
        QPointF p4(p3.x() + 15, p3.y());
        QLineF arrowLine3(p3, p4);
        arrowLine3.setAngle(line.angle() + 45);
        QLineF arrowLine4(p3, p4);
        arrowLine4.setAngle(line.angle() - 45);

        sourceArrowLine1 = new QGraphicsLineItem(arrowLine3);
        sourceArrowLine2 = new QGraphicsLineItem(arrowLine4);

        shapePath->lineTo(arrowLine3.p2());
        shapePath->lineTo(pi);
        shapePath->lineTo(arrowLine4.p2());
    }
    else {
        shapePath->lineTo(pi);
        sourceArrowLine1 = NULL;
        sourceArrowLine2 = NULL;
    }

//    if (weight) {
//        p0 = line.pointAt(0.5);
//        QPointF pt(p0.x() + 5, p0.y());
//        QLineF textLine(p0, pt);
//        textLine.setAngle(line.angle() + 90);
//        weightText = new QGraphicsTextItem(QString::number(weight));
//        weightText->setX(textLine.p2().x());
//        weightText->setY(textLine.p2().y());
//        weightText->setRotation(line.angle());
//    }
//    else
//        weightText = NULL;
}

int GraphEdge::getWeight()
{
    return weight;
}

void GraphEdge::setWeight(int w)
{
    weight = w;
}

bool GraphEdge::isUndirected()
{
    return undirected;
}

void GraphEdge::setUndirected(bool _undirected)
{
    undirected = _undirected;
    createDrawing();
    update();
}

GraphNode* GraphEdge::getSourceNode()
{
    return source;
}

GraphNode* GraphEdge::getDestinationNode()
{
    return destination;
}

QRectF GraphEdge::boundingRect() const
{
    return shapePath->boundingRect();
}

QPainterPath GraphEdge::shape() const
{
    return *shapePath;
}

void GraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
//    painter->fillPath(shape(),QBrush(Qt::blue));
    QPen pen(Qt::black, 2);
    QBrush brush(Qt::red);
    painter->setPen(pen);
    painter->setBrush(brush);

//    QPointF sourceCenter = source->getCenter();
//    QPointF destinationCenter = destination->getCenter();
//    QLineF line(sourceCenter, destinationCenter);
//    bool drawArrow = line.length() > 2*source->getRadius() && line.length() > 2*destination->getRadius();

//    if (drawArrow) {
        mainLine->paint(painter, option, widget);

        destinationArrowLine1->paint(painter, option, widget);
        destinationArrowLine2->paint(painter, option, widget);

        if (undirected) {
            sourceArrowLine1->paint(painter, option, widget);
            sourceArrowLine2->paint(painter, option, widget);
        }
//    }

//    if (weightText)
//        painter->drawText(weightText->pos().toPoint(), QString::number(weight));
//        weightText->paint(painter, option, widget);
}

void GraphEdge::sourceUpdated()
{
    createDrawing();
    update();
}

void GraphEdge::destinationUpdated()
{
    createDrawing();
    update();
}

void GraphEdge::changeDirection()
{
    GraphNode* temp = source;
    source = destination;
    destination = temp;
    createDrawing();
    update();
}
