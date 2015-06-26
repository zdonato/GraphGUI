#include "changeedgedirectioncommand.h"

#include <QGraphicsScene>

ChangeEdgeDirectionCommand::ChangeEdgeDirectionCommand(GraphEdge* _edge, bool _nodesSwapped, bool _newUndirected, QUndoCommand* parent)
  :
    QUndoCommand(parent),
    edge(_edge),
    nodesSwapped(_nodesSwapped),
    oldUndirected(_edge->isUndirected()),
    newUndirected(_newUndirected)
{

}

ChangeEdgeDirectionCommand::~ChangeEdgeDirectionCommand()
{

}

void ChangeEdgeDirectionCommand::undo()
{
    edge->setUndirected(oldUndirected);

    if (nodesSwapped)
        edge->changeDirection();

    edge->scene()->update();
}

void ChangeEdgeDirectionCommand::redo()
{
    edge->setUndirected(newUndirected);

    if (nodesSwapped)
        edge->changeDirection();

    edge->scene()->update();
}
