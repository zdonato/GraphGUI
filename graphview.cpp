#include <QColor>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>

#include "graphEdge.h"
#include "graphNode.h"
#include "graphview.h"

GraphView::GraphView(QWidget *parent)
  :
    QGraphicsView(parent),
    selectedItem(NULL),
    edgeSource(NULL),
    currentAction(NONE),
    nodeRadius(15)
{
    setScene(new QGraphicsScene());
    scene()->setSceneRect(this->rect());
}

GraphView::~GraphView()
{

}

void GraphView::setNodeRadius(int radius)
{
    nodeRadius = radius;

    QListIterator<QGraphicsItem*> it(scene()->items());
    while (it.hasNext()) {
        GraphNode* node = dynamic_cast<GraphNode*>(it.next());
        if (node)
            node->setRadius(radius);
    }
}

void GraphView::setCurrentAction(GraphAction action)
{
    currentAction = action;
}

void GraphView::executeContextMenu(const QPoint& menuPosition)
{
    QMenu menu;

    GraphNode* node = dynamic_cast<GraphNode*>(selectedItem);
    GraphEdge* edge = dynamic_cast<GraphEdge*>(selectedItem);

    if (node) {
        QAction *changeLabelAction = menu.addAction("Change label");
        QAction *deleteAction = menu.addAction("Delete vertex");
        QAction *selectedAction = menu.exec(menuPosition);

        if (selectedAction == changeLabelAction)
          changeLabel(node);
        else if (selectedAction == deleteAction)
          removeItem(node);
    }
    else if (edge){
        QAction *changeWeightAction = menu.addAction("Change weight");
        QAction *deleteAction = menu.addAction("Delete edge");
        QAction *selectedAction = menu.exec(menuPosition);

        if (selectedAction == changeWeightAction)
          changeWeight(edge);
        else if (selectedAction == deleteAction)
          removeItem(edge);
    }
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    QPointF pt = mapToScene(event->pos());

    selectedItem = scene()->itemAt(pt, QTransform());

    if (selectedItem && event->button() == Qt::RightButton) {
        executeContextMenu(event->screenPos().toPoint());
    }
    else {
        switch (currentAction) {
            case ADD_VERTEX:
            {
                GraphNode* node = new GraphNode(pt.x(), pt.y(), qreal(nodeRadius));
                scene()->addItem(node);
            }
            break;

            case ADD_DIRECTED_EDGE:
                if (edgeSource) {
                    GraphNode* edgeDestination = dynamic_cast<GraphNode*>(selectedItem);
                    if (edgeDestination && edgeSource != edgeDestination) {
                        QPointF sourceCenter = edgeSource->getCenter();
                        QPointF destinationCenter = edgeDestination->getCenter();
                        GraphEdge* edge = new GraphEdge(sourceCenter.x(), sourceCenter.y(), destinationCenter.x(), destinationCenter.y());
                        edgeSource->addSourceEdge(edge);
                        edgeDestination->addDestinationEdge(edge);
                        scene()->addItem(edge);
                        edgeSource = NULL;
                    }
                }
                else
                    edgeSource = dynamic_cast<GraphNode*>(selectedItem);
            break;

            case ADD_UNDIRECTED_EDGE:

            break;

            default:{}
        }
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

void GraphView::removeItem(GraphNode* node)
{
//    scene->removeItem(edge);
}

void GraphView::removeItem(GraphEdge* edge)
{
    scene()->removeItem(edge);
}

void GraphView::changeLabel(GraphNode* node)
{
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Change Label", "New label:", QLineEdit::Normal, "", &ok);
    if (ok)
      node->setLabel(newLabel);
}

void GraphView::changeWeight(GraphEdge* edge)
{

}
