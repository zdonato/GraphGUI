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
    scene()->setSceneRect(rect());
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

        QMenu *changeDirectionMenu = new QMenu("Change direction");
        menu.addMenu(changeDirectionMenu);
        QString sourceLabel = edge->getSourceNode()->getLabel();
        if (sourceLabel.isEmpty())
            sourceLabel = "no_label";
        QString destinationLabel = edge->getDestinationNode()->getLabel();
        if (destinationLabel.isEmpty())
            destinationLabel = "no_label";

        QAction *changeDirectionSDAction = changeDirectionMenu->addAction(sourceLabel + " -> " + destinationLabel);
        QAction *changeDirectionDSAction = changeDirectionMenu->addAction(destinationLabel + " -> " + sourceLabel);
        QAction *changeUndirectedAction = changeDirectionMenu->addAction("Change to undirected");

        if (edge->isUndirected())
            changeUndirectedAction->setEnabled(false);
        else
            changeDirectionSDAction->setEnabled(false);

        QAction *deleteAction = menu.addAction("Delete edge");
        QAction *selectedAction = menu.exec(menuPosition);

        if (selectedAction == changeWeightAction)
            changeWeight(edge);
        else if (selectedAction == deleteAction)
            removeItem(edge);
        else if (selectedAction == changeDirectionSDAction)
            edge->setUndirected(false);
        else if (selectedAction == changeDirectionDSAction) {
            edge->setUndirected(false);
            edge->changeDirection();
        }
        else if (selectedAction == changeUndirectedAction)
            edge->setUndirected(true);
    }
}

void GraphView::addEdge(bool undirected)
{
    if (edgeSource) {
        GraphNode* edgeDestination = dynamic_cast<GraphNode*>(selectedItem);
        if (edgeDestination && edgeSource != edgeDestination) {
            GraphEdge* edge = new GraphEdge(edgeSource, edgeDestination, undirected);
            edgeSource->addSourceEdge(edge);
            edgeDestination->addDestinationEdge(edge);
            scene()->addItem(edge);
            edgeSource = NULL;
        }
    }
    else
        edgeSource = dynamic_cast<GraphNode*>(selectedItem);
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
                addEdge(false);
            break;

            case ADD_UNDIRECTED_EDGE:
                addEdge(true);
            break;

            default:{}
        }
    }
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{
    if (currentAction == MOVING && selectedItem && event->button() != Qt::RightButton) {
        QPointF pt = mapToScene(event->pos());
        GraphNode* node = dynamic_cast<GraphNode*>(selectedItem);
        if (node) {
            QRect rect = this->geometry();

            if (event->pos().x() - node->getRadius() > rect.left() && event->pos().x() + node->getRadius() < rect.right() &&
                event->pos().y() - node->getRadius() > rect.top() && event->pos().y() + node->getRadius() < rect.bottom())
                node->moveTo(pt);
        }
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
    edge->getSourceNode()->addSourceEdge(NULL);
    edge->getDestinationNode()->addDestinationEdge(NULL);
    scene()->removeItem(edge);
    delete edge;
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
