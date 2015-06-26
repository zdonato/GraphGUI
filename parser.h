#ifndef PARSER_H
#define PARSER_H

#include "graphnode.h"
#include "graphedge.h"

class Parser
{
  public:
    Parser();
    virtual ~Parser();

    void parseGraph(QString filePath);
    void saveGraph(QString existingPath = NULL);

    QList<GraphNode*>& getNodes();
    void setNodes(QList<GraphNode*> nodes);

    void setEdgeType(bool type);
    void setNodeRadius(int r);
    void setGraphWeight(bool weight);

  private:
    GraphNode* findNode(unsigned id);

    QString edgeType;
    QString graphWeight;
    int nodeRadius;
    QList<GraphNode*> nodes;
    int numberOfNodes;
};

#endif // PARSER_H

