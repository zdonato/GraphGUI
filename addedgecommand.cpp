#include "addedgecommand.h"

#include "graphnode.h"

AddEdgeCommand::AddEdgeCommand(GraphEdge* _edge, GraphView *view, QUndoCommand *parent)
  :
    QUndoCommand(parent),
    edge(_edge),
    graphView(view)
{

}

AddEdgeCommand::~AddEdgeCommand()
{

}

void AddEdgeCommand::undo()
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

void AddEdgeCommand::redo()
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
