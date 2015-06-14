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
    status(NULL),
    nodeRadius(15),
    weightedGraph(false),
    undirectedGraph(false),
    numberOfNodes(0),
    numberOfEdges(0)
{
    setScene(new QGraphicsScene());
    scene()->setSceneRect(rect());
}

GraphView::~GraphView()
{

}

void GraphView::clear()
{
    scene()->clear();
    numberOfNodes = 0;
    numberOfEdges = 0;
    status->setText("");
    currentAction = NONE;
    edgeSource = NULL;
    selectedItem = NULL;
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

    // After changing the radius of every node, we recalculate the edges
    it.toFront();
    while (it.hasNext()) {
        GraphEdge* edge = dynamic_cast<GraphEdge*>(it.next());

        if (edge)
            edge->updateDrawing();
    }

    scene()->update();
}

void GraphView::setGraphWeighted(bool weighted)
{
    weightedGraph = weighted;
}

void GraphView::setGraphUndirected(bool undirected)
{
    undirectedGraph = undirected;
}

void GraphView::setCurrentAction(GraphAction action)
{
    currentAction = action;
    selectedItem = NULL;
    edgeSource = NULL;
    updateStatus();
}

void GraphView::setStatus(QLabel* _status)
{
    status = _status;
}

QString GraphView::actionString(GraphAction a)
{
    QString s;
    switch (a) {
        case MOVING:
            s = "Moving";
        break;

        case ADD_DIRECTED_EDGE:
            s = "Adding directed edge: ";
            if (edgeSource)
                s += "selecting destination from " + edgeSource->getLabel();
            else
                s += "selecting source";
        break;

        case ADD_UNDIRECTED_EDGE:
            s = "Adding undirected edge: ";
            if (a == ADD_UNDIRECTED_EDGE) {
                if (edgeSource)
                    s += "selecting second vertex";
                else
                    s += "selecting first vertex";
            }
        break;

        case ADD_VERTEX:
            s = "Adding vertex";
        break;

        case NONE:
            s = "No action";
        break;
    }

    return s;
}

void GraphView::updateStatus()
{
    QString statusMessage("");
    if (weightedGraph)
        statusMessage += "Weighted";
    else
        statusMessage += "Unweighted";

    if (undirectedGraph)
        statusMessage += " undirected";
    else
        statusMessage += " directed";

    statusMessage += " graph: " +
                     QString::number(numberOfNodes) + (numberOfNodes == 1 ? " vertex, " : " vertices, ") +
                     QString::number(numberOfEdges) + (numberOfEdges == 1 ? " edge" : " edges");

    if (currentAction != NONE)
        statusMessage += " - " + actionString(currentAction);

    status->setText(statusMessage);
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
        QAction *changeWeightAction(NULL);

        if (weightedGraph)
            changeWeightAction = menu.addAction("Change weight");

        QAction *changeDirectionSDAction(NULL);
        QAction *changeDirectionDSAction(NULL);
        QAction *changeUndirectedAction(NULL);

        if (!undirectedGraph) {
            QMenu *changeDirectionMenu = new QMenu("Change direction");
            menu.addMenu(changeDirectionMenu);
            QString sourceLabel = edge->getSourceNode()->getLabel();
            if (sourceLabel.isEmpty())
                sourceLabel = "no_label";
            QString destinationLabel = edge->getDestinationNode()->getLabel();
            if (destinationLabel.isEmpty())
                destinationLabel = "no_label";

            changeDirectionSDAction = changeDirectionMenu->addAction(sourceLabel + " -> " + destinationLabel);
            changeDirectionDSAction = changeDirectionMenu->addAction(destinationLabel + " -> " + sourceLabel);
            changeUndirectedAction = changeDirectionMenu->addAction("Change to undirected");

            if (edge->isUndirected())
                changeUndirectedAction->setEnabled(false);
            else
                changeDirectionSDAction->setEnabled(false);
        }

        QAction *deleteAction = menu.addAction("Delete edge");
        QAction *selectedAction = menu.exec(menuPosition);

        if (selectedAction) {
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

            scene()->update();
        }
    }
}

void GraphView::addEdge(bool undirected)
{
    if (edgeSource) {
        GraphNode* edgeDestination = dynamic_cast<GraphNode*>(selectedItem);
        if (edgeDestination && edgeSource != edgeDestination) {
            GraphEdge* existingEdge = NULL;
            bool drawAsArc(false);

            QListIterator<GraphEdge*> it(edgeSource->getDestinationEdges());
            while (it.hasNext() && !existingEdge) {
                GraphEdge* edge = it.next();
                if (edge->getSourceNode() == edgeDestination)
                    existingEdge = edge;
            }

            if (existingEdge) {
                if (!existingEdge->isUndirected()) {
                    if (weightedGraph && !undirectedGraph && currentAction == ADD_DIRECTED_EDGE) {
                        existingEdge->setDrawAsArc(true);
                        drawAsArc = true;
                        existingEdge = NULL;
                    }
                    else {
                        existingEdge->setUndirected(true);
                        edgeSource->addSourceEdge(existingEdge);
                        edgeDestination->addDestinationEdge(existingEdge);
                    }
                }
            }
            else {
                it = QListIterator<GraphEdge*>(edgeSource->getSourceEdges());
                while (it.hasNext() && !existingEdge) {
                    GraphEdge* edge = it.next();
                    if (edge->getDestinationNode() == edgeDestination) {
                       existingEdge = edge;
                       if (currentAction == ADD_UNDIRECTED_EDGE) {
                           existingEdge->setUndirected(true);
                           edgeSource->addDestinationEdge(existingEdge);
                           edgeDestination->addSourceEdge(existingEdge);
                       }
                    }
                }
            }

            if (!existingEdge) {
                GraphEdge* edge = new GraphEdge(edgeSource, edgeDestination, undirected, !undirectedGraph, weightedGraph, drawAsArc);
                edgeSource->addSourceEdge(edge);
                edgeDestination->addDestinationEdge(edge);

                if (undirected) {
                    edgeSource->addDestinationEdge(edge);
                    edgeDestination->addSourceEdge(edge);
                }

                scene()->addItem(edge);
                ++numberOfEdges;
                updateStatus();
            }

            edgeSource = NULL;
            scene()->update();
        }
    }
    else
        edgeSource = dynamic_cast<GraphNode*>(selectedItem);

    updateStatus();
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
                ++numberOfNodes;
                GraphNode* node = new GraphNode(numberOfNodes, pt.x(), pt.y(), qreal(nodeRadius));
                scene()->addItem(node);
                updateStatus();
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
                event->pos().y() - node->getRadius() > rect.top() && event->pos().y() + node->getRadius() < rect.bottom()) {
                node->moveTo(pt);            
                scene()->update();
            }
        }
    }    
}

void GraphView::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    selectedItem = NULL;
}

void GraphView::removeItem(GraphNode* node)
{
    QList<GraphEdge*>& edges = node->getSourceEdges();
    QListIterator<GraphEdge*> it(edges);

    while (it.hasNext())
        removeItem(it.next());

    edges = node->getDestinationEdges();
    it = QListIterator<GraphEdge*>(edges);

    while (it.hasNext())
        removeItem(it.next());

    scene()->removeItem(node);
    --numberOfNodes;
    updateStatus();
    delete node;
}

void GraphView::removeItem(GraphEdge* edge)
{
    edge->getSourceNode()->removeSourceEdge(edge);
    edge->getDestinationNode()->removeDestinationEdge(edge);

    if (edge->isUndirected()) {
        edge->getSourceNode()->removeDestinationEdge(edge);
        edge->getDestinationNode()->removeSourceEdge(edge);
    }

    scene()->removeItem(edge);
    --numberOfEdges;
    updateStatus();
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
    bool ok;
    int newWeight = QInputDialog::getInt(this, "Change Label", "New weight:", edge->getWeight(), 0, 999999, 1, &ok);
    if (ok) {
        edge->setWeight(newWeight);
        edge->update();
    }
}
