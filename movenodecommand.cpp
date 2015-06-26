#include <QGraphicsScene>

#include "movenodecommand.h"

MoveNodeCommand::MoveNodeCommand(GraphNode* _node, const QPointF& _oldPos, const QPointF& _newPos, QUndoCommand* parent)
  :
    QUndoCommand(parent),
    node(_node),
    oldPos(_oldPos),
    newPos(_newPos)
{

}

MoveNodeCommand::~MoveNodeCommand()
{

}

void MoveNodeCommand::undo()
{
     node->moveTo(oldPos);
     node->scene()->update();
}

void MoveNodeCommand::redo()
{
    node->moveTo(newPos);
    node->scene()->update();
}

bool MoveNodeCommand::mergeWith(const QUndoCommand *command)
{
    const MoveNodeCommand* moveCmd = static_cast<const MoveNodeCommand*>(command);
    GraphNode* otherNode = moveCmd->node;

    if (otherNode != node)
        return false;

    newPos = otherNode->getCenter();

    return true;
}
