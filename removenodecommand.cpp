#include "removenodecommand.h"

#include <QGraphicsScene>

RemoveNodeCommand::RemoveNodeCommand(GraphNode* _node, GraphView *view, QUndoCommand *parent)
  :
    QUndoCommand(parent),
    node(_node),
    graphView(view)
{

}

RemoveNodeCommand::~RemoveNodeCommand()
{

}

void RemoveNodeCommand::undo()
{
    graphView->addNode(node);
}

void RemoveNodeCommand::redo()
{
    graphView->removeNode(node);
}

