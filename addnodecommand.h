#ifndef ADDNODECOMMAND_H
#define ADDNODECOMMAND_H

#include <QUndoCommand>

#include "graphnode.h"
#include "graphview.h"

class AddNodeCommand : public QUndoCommand
{
  public:
    AddNodeCommand(GraphNode* node, GraphView *view, QUndoCommand *parent = 0);

    virtual ~AddNodeCommand();

    void undo();
    void redo();

  private:
    GraphNode *node;
    GraphView *graphView;
};

#endif // ADDNODECOMMAND_H
