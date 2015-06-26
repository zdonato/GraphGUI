#ifndef CHANGEEDGEDIRECTIONCOMMAND_H
#define CHANGEEDGEDIRECTIONCOMMAND_H

#include <QUndoCommand>

#include "graphedge.h"

class ChangeEdgeDirectionCommand : public QUndoCommand
{
  public:
    ChangeEdgeDirectionCommand(GraphEdge* edge, bool nodesSwapped, bool newUndirected, QUndoCommand* parent = 0);

    virtual ~ChangeEdgeDirectionCommand();

    void undo();
    void redo();

  private:
    GraphEdge* edge;
    bool nodesSwapped;
    bool oldUndirected;
    bool newUndirected;
};

#endif // CHANGEEDGEDIRECTIONCOMMAND_H
