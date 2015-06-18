#ifndef PARSER_H
#define PARSER_H

#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "graphnode.h"
#include "graphedge.h"

class Parser
{
public:
    Parser();

    void parseGraph(QString filePath);
    void saveGraph(QString existingPath = NULL);

    std::string edgeType;
    std::string graphWeight;
    int nodeRadius;
    QList<GraphNode*> nodes;

private:
    GraphNode findNode(int id);
};

#endif // PARSER_H

