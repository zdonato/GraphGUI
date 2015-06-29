#ifndef CHANGENODELABELCOMMAND_H
#define CHANGENODELABELCOMMAND_H

#include <QUndoCommand>

#include "graphnode.h"

class ChangeNodeLabelCommand : public QUndoCommand
{
public:
    ChangeNodeLabelCommand(GraphNode* node, const QString& newLabel, QUndoCommand* parent = 0);

    virtual ~ChangeNodeLabelCommand();

    void undo();
    void redo();

  private:
    GraphNode* node;
    QString oldLabel;
    QString newLabel;
};

#endif // CHANGENODELABELCOMMAND_H
