#ifndef REMOVENODECOMMAND_H
#define REMOVENODECOMMAND_H

#include <QUndoCommand>

#include "graphnode.h"
#include "graphview.h"

class RemoveNodeCommand : public QUndoCommand
{
  public:
    RemoveNodeCommand(GraphNode* node, GraphView *view, QUndoCommand *parent = 0);

    virtual ~RemoveNodeCommand();

    void undo();
    void redo();

  private:
    GraphNode* node;
    GraphView* graphView;
};

#endif // REMOVENODECOMMAND_H
