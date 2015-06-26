#include "changeedgeweightcommand.h"

#include <QGraphicsScene>

ChangeEdgeWeightCommand::ChangeEdgeWeightCommand(GraphEdge* _edge, int _newWeight, QUndoCommand* parent)
  :
    QUndoCommand(parent),
    edge(_edge),
    oldWeight(edge->getWeight()),
    newWeight(_newWeight)
{

}

ChangeEdgeWeightCommand::~ChangeEdgeWeightCommand()
{

}

void ChangeEdgeWeightCommand::undo()
{
    edge->setWeight(oldWeight);
    edge->scene()->update();
}

void ChangeEdgeWeightCommand::redo()
{
    edge->setWeight(newWeight);
    edge->scene()->update();
}
