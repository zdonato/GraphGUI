#include "changenodelabelcommand.h"

ChangeNodeLabelCommand::ChangeNodeLabelCommand(GraphNode* _node, const QString& _newLabel, QUndoCommand *parent)
  :
    QUndoCommand(parent),
    node(_node),
    oldLabel(_node->getLabel()),
    newLabel(_newLabel)
{

}

ChangeNodeLabelCommand::~ChangeNodeLabelCommand()
{

}

void ChangeNodeLabelCommand::undo()
{
    node->setLabel(oldLabel);
    node->update();
}

void ChangeNodeLabelCommand::redo()
{
    node->setLabel(newLabel);
    node->update();
}
