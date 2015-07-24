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

QList<GraphEdge*>& Parser::getEdges()
{
    return edges;
}

void Parser::setEdges(QList<GraphEdge *> edges)
{
    this->edges = edges;
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

bool Parser::isGraphWeighted()
{
    return graphWeight == "weighted";
}

bool Parser::isGraphUndirected()
{
    return edgeType == "undirected";
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
                // Load the values.

                // Load the edge type.
                rapidjson::Value& val = d["edgeType"];
                edgeType = val.GetString();

                // Load the graph weight.
                val = d["graphWeight"];
                graphWeight = val.GetString();

                // Load the radius.
                val = d["radius"];
                nodeRadius = val.GetInt();

                // Load the nodes.
                const rapidjson::Value& nodesArray = d["nodes"];
                assert(nodesArray.IsArray());

                for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i) {
                    GraphNode* node = new GraphNode(nodesArray[i]["id"].GetInt(),
                                                    nodesArray[i]["x"].GetInt(),
                                                    nodesArray[i]["y"].GetInt(),
                                                    nodeRadius);
                    node->setLabel(nodesArray[i]["label"].GetString());
                    nodes.append(node);
                }

                // Load the edges.

                const rapidjson::Value& edgesArray = d["edges"];
                assert(edgesArray.IsArray());

                for (rapidjson::SizeType i = 0; i < edgesArray.Size(); ++i){
                    GraphEdge* edge = new GraphEdge(findNode(edgesArray[i]["source"].GetInt()),
                                                    findNode(edgesArray[i]["destination"].GetInt()),
                                                    edgesArray[i]["undirected"].GetBool(),
                                                    edgesArray[i]["drawArrows"].GetBool(),
                                                    edgesArray[i]["drawWeight"].GetBool(),
                                                    edgesArray[i]["drawAsArc"].GetBool());
                    edges.append(edge);
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

    // Save each node.
    while (it.hasNext()) {
        GraphNode* node = it.next();

        writer.StartObject();

        writer.String("label");
        writer.String(node->getLabel().toStdString().c_str());

        QPointF nodeCenter = node->getCenter();

        writer.String("x");
        writer.Int(nodeCenter.x());

        writer.String("y");
        writer.Int(nodeCenter.y());

        writer.String("id");
        writer.Int(node->getId());

        writer.EndObject();
    }

    writer.EndArray();

    QListIterator<GraphEdge*> itE(getEdges());

    // Save all edges.

    writer.String("edges");
    writer.StartArray();

    while(itE.hasNext()){
        GraphEdge* edge = itE.next();

        writer.StartObject();

        writer.String("source");
        writer.Int(edge->getSourceNode()->getId());

        writer.String("destination");
        writer.Int(edge->getDestinationNode()->getId());

        writer.String("weight");
        writer.Int(edge->getWeight());

        writer.String("undirected");
        writer.Bool(edge->isUndirected());

        writer.String("isBold");
        writer.Bool(edge->isBold());

        writer.String("drawArrows");
        writer.Bool(edge->shouldDrawArrows());

        writer.String("drawWeight");
        writer.Bool(edge->shouldDrawWeight());

        writer.String("drawAsArc");
        writer.Bool(edge->shouldDrawAsArc());

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
