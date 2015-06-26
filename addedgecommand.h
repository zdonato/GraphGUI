#ifndef ADDEDGECOMMAND_H
#define ADDEDGECOMMAND_H

#include <QUndoCommand>

#include "graphedge.h"
#include "graphview.h"

class AddEdgeCommand : public QUndoCommand
{
  public:
    AddEdgeCommand(GraphEdge* node, GraphView *view, QUndoCommand *parent = 0);

    virtual ~AddEdgeCommand();

    void undo();
    void redo();

  private:
    GraphEdge *edge;
    GraphView *graphView;
};

#endif // ADDEDGECOMMAND_H
