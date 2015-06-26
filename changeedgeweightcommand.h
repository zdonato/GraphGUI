#ifndef CHANGEEDGEWEIGHTCOMMAND_H
#define CHANGEEDGEWEIGHTCOMMAND_H

#include <QUndoCommand>

#include "graphedge.h"

class ChangeEdgeWeightCommand : public QUndoCommand
{
public:
    ChangeEdgeWeightCommand(GraphEdge* edge, int newWeight, QUndoCommand* parent = 0);

    virtual ~ChangeEdgeWeightCommand();

    void undo();
    void redo();

  private:
    GraphEdge* edge;
    int oldWeight;
    int newWeight;
};

#endif // CHANGEEDGEWEIGHTCOMMAND_H
