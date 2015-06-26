#include "addnodecommand.h"

AddNodeCommand::AddNodeCommand(GraphNode* _node, GraphView *view, QUndoCommand *parent)
  :
    QUndoCommand(parent),
    node(_node),
    graphView(view)
{

}

AddNodeCommand::~AddNodeCommand()
{

}

void AddNodeCommand::undo()
{
    graphView->removeNode(node);
}

void AddNodeCommand::redo()
{
    graphView->addNode(node);
}
