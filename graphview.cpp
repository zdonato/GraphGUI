#include <QColor>

#include "graphEdge.h"
#include "graphNode.h"
#include "graphview.h"

GraphView::GraphView(QWidget *parent) : QGraphicsView(parent), selectedItem(NULL), edgeSource(NULL)
{
    scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(this->rect());
}

GraphView::~GraphView()
{
    delete scene;
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    QPointF pt = mapToScene(event->pos());

    selectedItem = scene->itemAt(pt, QTransform());

    if (selectedItem) {
        if (event->button() == Qt::RightButton) {
            if (edgeSource) {
                GraphNode* edgeDestination = dynamic_cast<GraphNode*>(selectedItem);
                if (edgeDestination && edgeSource != edgeDestination) {
                    QPointF sourceCenter = edgeSource->getCenter();
                    QPointF destinationCenter = edgeDestination->getCenter();
                    GraphEdge* edge = new GraphEdge(sourceCenter.x(), sourceCenter.y(), destinationCenter.x(), destinationCenter.y());
                    edgeSource->addSourceEdge(edge);
                    edgeDestination->addDestinationEdge(edge);
                    scene->addItem(edge);
                    edgeSource = NULL;
                }
            }
            else
                edgeSource = dynamic_cast<GraphNode*>(selectedItem);
        }
//        GraphNode* node = dynamic_cast<GraphNode*>(item);
//        node->setPressed(true);
    }
    else {
        GraphNode* node = new GraphNode(pt.x(), pt.y(), qreal(10));
        scene->addItem(node);
    }
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{
    if (selectedItem && event->button() != Qt::RightButton) {
        QPointF pt = mapToScene(event->pos());
        GraphNode* node = dynamic_cast<GraphNode*>(selectedItem);
        if (node)
            node->moveTo(pt);
    }
}

void GraphView::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    selectedItem = NULL;
}
