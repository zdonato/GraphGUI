#include "removeedgecommand.h"

#include <QGraphicsScene>

#include "graphnode.h"

RemoveEdgeCommand::RemoveEdgeCommand(GraphEdge* _edge, GraphView *view, QUndoCommand *parent)
  :
    QUndoCommand(parent),
    edge(_edge),
    graphView(view)
{

}

RemoveEdgeCommand::~RemoveEdgeCommand()
{

}

void RemoveEdgeCommand::undo()
{
    graphView->addEdge(edge);

    edge->getSourceNode()->addSourceEdge(edge);
    edge->getDestinationNode()->addDestinationEdge(edge);

    if (edge->isUndirected()) {
        edge->getSourceNode()->addDestinationEdge(edge);
        edge->getDestinationNode()->addSourceEdge(edge);
    }

    graphView->scene()->update();
}

void RemoveEdgeCommand::redo()
{
    graphView->removeEdge(edge);

    edge->getSourceNode()->removeSourceEdge(edge);
    edge->getDestinationNode()->removeDestinationEdge(edge);

    if (edge->isUndirected()) {
        edge->getSourceNode()->removeDestinationEdge(edge);
        edge->getDestinationNode()->removeSourceEdge(edge);
    }

    graphView->scene()->update();
}
