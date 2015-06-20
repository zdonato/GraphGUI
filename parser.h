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

    QList<GraphNode*>& getNodes();

private:
    GraphNode* findNode(unsigned id);

    QString edgeType;
    QString graphWeight;
    int nodeRadius;
    QList<GraphNode*> nodes;
};

#endif // PARSER_H

