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
    QList<GraphEdge*>& getEdges();
    void setNodes(QList<GraphNode*> nodes);
    void setEdges(QList<GraphEdge*> edges);

    void setEdgeType(bool type);
    void setNodeRadius(int r);
    void setGraphWeight(bool weight);

  private:
    GraphNode* findNode(unsigned id);

    QString edgeType;
    QString graphWeight;
    int nodeRadius;
    QList<GraphNode*> nodes;
    QList<GraphEdge*> edges;
};

#endif // PARSER_H

