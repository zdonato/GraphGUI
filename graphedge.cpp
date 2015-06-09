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
  weight(0),
  bold(false),
  drawArrows(true),
  drawWeight(true)
{
    updateDrawing();
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

void GraphEdge::updateDrawing()
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

    mainLine = new QGraphicsLineItem(line);

    if (drawArrows) {
        QPointF p1(pf);
        QPointF p2(p1.x() + 13, p1.y());
        QLineF arrowLine1(p1, p2);
        arrowLine1.setAngle(line.angle() - 135);
        QLineF arrowLine2(p1, p2);
        arrowLine2.setAngle(line.angle() + 135);

        destinationArrowLine1 = new QGraphicsLineItem(arrowLine1);
        destinationArrowLine2 = new QGraphicsLineItem(arrowLine2);

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
    }
    else {
        shapePath->moveTo(pf);
        shapePath->lineTo(pi);
        destinationArrowLine1 = NULL;
        destinationArrowLine2 = NULL;
        sourceArrowLine1 = NULL;
        sourceArrowLine2 = NULL;
    }

    if (drawWeight) {
        p0 = line.pointAt(0.5);
        QPointF pt(p0.x() + 13, p0.y());
        QLineF textLine(p0, pt);
        qreal lineAngle = line.angle();
    //        if (lineAngle > 90 && lineAngle < 270)
    //            lineAngle += 180;

        textLine.setAngle(lineAngle + 90);
        weightText = new QGraphicsTextItem(QString::number(weight));
        weightText->setX(textLine.p2().x());
        weightText->setY(textLine.p2().y());
    //        weightText->setRotation(lineAngle);
    }
    else
        weightText = NULL;
}

int GraphEdge::getWeight()
{
    return weight;
}

void GraphEdge::setWeight(int w)
{
    weight = w;
    weightText->setPlainText(QString::number(weight));
}

bool GraphEdge::isUndirected()
{
    return undirected;
}

void GraphEdge::setUndirected(bool _undirected)
{
    undirected = _undirected;
    updateDrawing();
}

void GraphEdge::setBold(bool _bold)
{
    bold = _bold;
}

void GraphEdge::setDrawArrows(bool draw)
{
    drawArrows = draw;
}

void GraphEdge::setDrawWeight(bool draw)
{
    drawWeight = draw;
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
    QPen pen(Qt::black, bold ? 4 : 2);
    painter->setPen(pen);

    mainLine->paint(painter, option, widget);

    if (drawArrows) {
        destinationArrowLine1->paint(painter, option, widget);
        destinationArrowLine2->paint(painter, option, widget);

        if (undirected) {
            sourceArrowLine1->paint(painter, option, widget);
            sourceArrowLine2->paint(painter, option, widget);
        }
    }

    if (drawWeight) {
        painter->save();
        painter->translate(weightText->pos().toPoint());
//        painter->rotate(-weightText->rotation());
//        QRectF textRect = weightText->boundingRect();
//        painter->translate(-textRect.top() - 0.5*textRect.width(), -textRect.left());
//        textRect.moveTo(0 - 0.5*textRect.width(), 0);
//        painter->drawText(textRect, Qt::AlignCenter, QString::number(weight));
        painter->drawText(0 - 0.35*weightText->boundingRect().width(), 0 + 0.2*weightText->boundingRect().height(), QString::number(weight));
        painter->restore();
//        weightText->paint(painter, option, widget);
    }
}

void GraphEdge::sourceUpdated()
{
    updateDrawing();
}

void GraphEdge::destinationUpdated()
{
    updateDrawing();
}

void GraphEdge::changeDirection()
{
    GraphNode* temp = source;
    source = destination;
    destination = temp;
    updateDrawing();
}
