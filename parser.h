#ifndef PARSER_H
#define PARSER_H

#include "graphnode.h"
#include "graphedge.h"

class Parser
{
public:
    Parser();

    void parseGraph(QString filePath);
    void saveGraph(QString existingPath = NULL);
    void setNodes(QList<GraphNode*> nodes);
    void setEdgeType(QString type);
    void setRadius(int r);
    void setGraphWeight(QString weight);

    QList<GraphNode*>& getNodes();

private:
    GraphNode* findNode(unsigned id);

    QString edgeType;
    QString graphWeight;
    int nodeRadius;
    QList<GraphNode*> nodes;
    int numberNodes;
};

#endif // PARSER_H

