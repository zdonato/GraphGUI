#ifndef MOVENODECOMMAND_H
#define MOVENODECOMMAND_H

#include <QUndoCommand>

#include "graphnode.h"

class MoveNodeCommand : public QUndoCommand
{
  public:
    enum { Id = 1234 };

    MoveNodeCommand(GraphNode* node, const QPointF &oldPos, const QPointF &newPos, QUndoCommand *parent = 0);
    virtual ~MoveNodeCommand();

    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const { return Id; }

  private:
    GraphNode* node;
    QPointF oldPos;
    QPointF newPos;
 };

#endif // MOVENODECOMMAND_H
