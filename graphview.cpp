#include <QColor>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>

#include "addedgecommand.h"
#include "addnodecommand.h"
#include "changeedgedirectioncommand.h"
#include "changeedgeweightcommand.h"
#include "changenodelabelcommand.h"
#include "graphEdge.h"
#include "graphNode.h"
#include "graphview.h"
#include "movenodecommand.h"
#include "removeedgecommand.h"
#include "removenodecommand.h"

GraphView::GraphView(QWidget *parent)
  :
    QGraphicsView(parent),
    selectedItem(NULL),
    edgeSource(NULL),
    currentAction(NONE),
    status(NULL),
    undoStack(NULL),
    nodeRadius(15),
    weightedGraph(false),
    undirectedGraph(false),
    numberOfNodes(0),
    numberOfEdges(0),
    nextNodeID(1)
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
    nextNodeID = 1;
    status->setText("");
    currentAction = NONE;
    edgeSource = NULL;
    selectedItem = NULL;
    undoStack->clear();
}

void GraphView::addNode(GraphNode* node)
{
    if (node->scene())
        node->setVisible(true);
    else
        scene()->addItem(node);

    ++numberOfNodes;
    updateStatus();
}

void GraphView::addEdge(GraphEdge* edge)
{
    if (edge->scene())
        edge->setVisible(true);
    else
        scene()->addItem(edge);

    ++numberOfEdges;
    updateStatus();
}

void GraphView::removeNode(GraphNode* node)
{
    node->setVisible(false);
    --numberOfNodes;
    updateStatus();
}

void GraphView::removeEdge(GraphEdge* edge)
{
    edge->setVisible(false);
    --numberOfEdges;
    updateStatus();
}

int GraphView::getNodeRadius()
{
    return nodeRadius;
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

bool GraphView::isGraphWeighted()
{
    return weightedGraph;
}

void GraphView::setGraphWeighted(bool weighted)
{
    weightedGraph = weighted;
}

bool GraphView::isGraphUndirected()
{
    return undirectedGraph;
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

void GraphView::setUndoStack(QUndoStack* stack)
{
    undoStack = stack;
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

QList<GraphNode*> GraphView::getNodes()
{
    QList<GraphNode*> nodes;

    QListIterator<QGraphicsItem*> it(scene()->items());
    while (it.hasNext()) {
        GraphNode* node = dynamic_cast<GraphNode*>(it.next());

        if (node->isVisible())
            nodes.append(node);
    }

    return nodes;
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
          removeItemCommand(node);
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
            if (selectedAction == deleteAction)
                removeItemCommand(edge);
            else if (selectedAction == changeWeightAction)
                changeWeight(edge);
            else if (selectedAction == changeDirectionSDAction)
                undoStack->push(new ChangeEdgeDirectionCommand(edge, false, false));
            else if (selectedAction == changeDirectionDSAction)
                undoStack->push(new ChangeEdgeDirectionCommand(edge, true, false));
            else if (selectedAction == changeUndirectedAction)
                undoStack->push(new ChangeEdgeDirectionCommand(edge, false, true));
        }
    }
}

void GraphView::addEdgeCommand(bool undirected)
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
                    else
                        undoStack->push(new ChangeEdgeDirectionCommand(existingEdge, false, true));
                }
            }
            else {
                it = QListIterator<GraphEdge*>(edgeSource->getSourceEdges());
                while (it.hasNext() && !existingEdge) {
                    GraphEdge* edge = it.next();
                    if (edge->getDestinationNode() == edgeDestination) {
                       existingEdge = edge;
                       if (currentAction == ADD_UNDIRECTED_EDGE)
                           undoStack->push(new ChangeEdgeDirectionCommand(existingEdge, false, true));
                    }
                }
            }

            if (!existingEdge) {
                GraphEdge* edge = new GraphEdge(edgeSource, edgeDestination, undirected, !undirectedGraph, weightedGraph, drawAsArc);
                undoStack->push(new AddEdgeCommand(edge, this));
            }

            edgeSource = NULL;
        }
    }
    else {
        edgeSource = dynamic_cast<GraphNode*>(selectedItem);
        updateStatus();
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
                GraphNode* node = new GraphNode(nextNodeID++, pt.x(), pt.y(), qreal(nodeRadius));
                undoStack->push(new AddNodeCommand(node, this));
            }
            break;

            case ADD_DIRECTED_EDGE:
                addEdgeCommand(false);
            break;

            case ADD_UNDIRECTED_EDGE:
                addEdgeCommand(true);
            break;

            default:{}
        }
    }
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{    
    if (currentAction == MOVING && selectedItem && event->button() != Qt::RightButton) {
        QPointF newPos = mapToScene(event->pos());
        GraphNode* node = dynamic_cast<GraphNode*>(selectedItem);
        if (node) {
            QRect rect = this->geometry();

            if (event->pos().x() - node->getRadius() > rect.left() && event->pos().x() + node->getRadius() < rect.right() &&
                event->pos().y() - node->getRadius() > rect.top() && event->pos().y() + node->getRadius() < rect.bottom()) {
                QPointF oldPos = node->getCenter();
                undoStack->push(new MoveNodeCommand(node, oldPos, newPos));
            }
        }
    }    
}

void GraphView::mouseReleaseEvent(QMouseEvent* /*event*/)
{
    selectedItem = NULL;
}

void GraphView::removeItemCommand(GraphNode* node)
{
    undoStack->beginMacro("");
    undoStack->push(new RemoveNodeCommand(node, this));

    QList<GraphEdge*>& edges = node->getSourceEdges();
    QListIterator<GraphEdge*> it(edges);

    while (it.hasNext())
        removeItemCommand(it.next());

    edges = node->getDestinationEdges();
    it = QListIterator<GraphEdge*>(edges);

    while (it.hasNext())
        removeItemCommand(it.next());

    undoStack->endMacro();
}

void GraphView::removeItemCommand(GraphEdge* edge)
{
    undoStack->push(new RemoveEdgeCommand(edge, this));
}

void GraphView::changeLabel(GraphNode* node)
{
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Change Label", "New label:", QLineEdit::Normal, "", &ok);
    if (ok)
        undoStack->push(new ChangeNodeLabelCommand(node, newLabel));
}

void GraphView::changeWeight(GraphEdge* edge)
{
    bool ok;
    int newWeight = QInputDialog::getInt(this, "Change Label", "New weight:", edge->getWeight(), 0, 999999, 1, &ok);
    if (ok)
        undoStack->push(new ChangeEdgeWeightCommand(edge, newWeight));
}
