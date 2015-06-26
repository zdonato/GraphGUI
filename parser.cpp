#include "parser.h"

#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Parser::Parser()
{

}

Parser::~Parser()
{

}

QList<GraphNode*>& Parser::getNodes()
{
    return nodes;
}

void Parser::setNodes(QList<GraphNode*> nodes)
{
    this->nodes = nodes;
}

void Parser::setEdgeType(bool undirected)
{
    if (undirected)
        edgeType = "undirected";
    else
        edgeType = "directed";
}

void Parser::setGraphWeight(bool weighted)
{
    if (weighted)
        graphWeight = "weighted";
    else
        graphWeight = "unweighted";
}

void Parser::setNodeRadius(int r)
{
    nodeRadius = r;
}

// Finds and returns the node with the given id. Returns null if not found.
GraphNode* Parser::findNode(unsigned id)
{
    GraphNode* result = NULL;

    QListIterator<GraphNode*> it(nodes);
    while (it.hasNext() && !result) {
        GraphNode* node = it.next();

        if (node->getId() == id)
            result = node;
    }

    return result;
}

void Parser::parseGraph(QString filePath)
{
    if (!filePath.isNull()) {
        QFileInfo fileInfo(filePath);
        const QString absPath = fileInfo.absoluteFilePath();

        if (!absPath.isNull()) {
            std::ifstream file(absPath.toStdString().c_str());

            if (file.is_open()) {
                std::string content( (std::istreambuf_iterator<char>(file) ),
                                     (std::istreambuf_iterator<char>() ));

                rapidjson::Document d;
                d.Parse(content.c_str());

                // We now have a DOM of the JSON.
                // Save the values.

                // Save the edge type.
                rapidjson::Value& val = d["edgeType"];
                edgeType = val.GetString();

                // Save the graph weight.
                val = d["graphWeight"];
                graphWeight = val.GetString();

                // Save the radius.
                val = d["radius"];
                nodeRadius = val.GetInt();

                // Save the nodes.
                const rapidjson::Value& nodesArray = d["nodes"];
                assert(nodesArray.IsArray());

                for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i) {
                    GraphNode* node = new GraphNode(nodesArray[i]["id"].GetInt(),
                                                    nodesArray[i]["x"].GetInt(),
                                                    nodesArray[i]["y"].GetInt(),
                                                    nodeRadius);
                    node->setLabel(nodesArray[i]["label"].GetString());

                    // Add any adjacent nodes.
                    const rapidjson::Value& adjArray = nodesArray[i]["adjacent"];
                    if (!adjArray.IsNull() && adjArray.IsArray()) {
                        for (rapidjson::SizeType j = 0; j < adjArray.Size(); ++j) {
                            GraphNode* adj = findNode(adjArray[i]["nodeID"].GetInt());

                            if (adj != NULL) {
                                // TODO: Fix whether undirected and weighted.
                                GraphEdge* edge = new GraphEdge(node, adj, true, true, false, false);
                                node->addDestinationEdge(edge);
                            }
                        }
                    }

                    nodes.append(node);
                }
            }
        }
    }
    else
        std::cout << "Unable to open file" << std::endl;
}

void Parser::saveGraph(QString filePath)
{
    // Save graph to this file name. Overwrite the contents of the file.
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    writer.StartObject();

    writer.String("edgeType");
    writer.String(edgeType.toStdString().c_str());

    writer.String("graphWeight");
    writer.String(graphWeight.toStdString().c_str());

    writer.String("radius");
    writer.Int(nodeRadius);

    // Write out the nodes.
    writer.String("nodes");
    writer.StartArray();

    QListIterator<GraphNode*> it(getNodes());

    while(it.hasNext()) {
        GraphNode* node = it.next();

        writer.StartObject();

        writer.String("label");
        writer.String(node->getLabel().toStdString().c_str());

        writer.String("x");
        writer.Int(node->x());

        writer.String("y");
        writer.Int(node->y());

        writer.String("id");
        writer.Int(node->getId());

        writer.EndObject();
    }

    writer.EndArray();

    writer.EndObject();

    if (!filePath.isNull()) {
        std::ofstream out_file;
        out_file.open(filePath.toStdString().c_str());

        out_file << s.GetString();
        out_file.close();
    }
    else {
        // Figure out what to do if no path given?
    }
}
