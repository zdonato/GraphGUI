#ifndef REMOVEEDGECOMMAND_H
#define REMOVEEDGECOMMAND_H

#include <QUndoCommand>

#include "graphedge.h"
#include "graphview.h"

class RemoveEdgeCommand : public QUndoCommand
{
  public:
    RemoveEdgeCommand(GraphEdge* edge, GraphView *view, QUndoCommand *parent = 0);

    virtual ~RemoveEdgeCommand();

    void undo();
    void redo();

  private:
    GraphEdge* edge;
    GraphView* graphView;
};

#endif // REMOVENODECOMMAND_H
