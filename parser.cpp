#include "parser.h"

Parser::Parser() { }

void Parser::parseGraph(QString filePath){

    if (!filePath.isNull()){

        QFileInfo fileInfo(filePath);
        const QString absPath = fileInfo.absoluteFilePath();

        if (!absPath.isNull()){

            std::ifstream file(absPath.toStdString().c_str());

            if (file.is_open()){
                std::string content( (std::istreambuf_iterator<char>(file) ),
                                     (std::istreambuf_iterator<char>() ));

                rapidjson::Document d;
                d.Parse(content.c_str());

                // We now have a DOM of the JSON.
                // Save the values.

                // Save the edge type.
                rapidjson::Value& val = d["edgeType"];
                this->edgeType = val.GetString();

                // Save the graph weight.
                val = d["graphWeight"];
                this->graphWeight = val.GetString();

                // Save the radius.
                val = d["radius"];
                this->nodeRadius = val.GetInt();

                // Save the nodes.
                const rapidjson::Value& nodesArray = d["nodes"];
                assert(nodesArray.IsArray());

                for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i){
                    GraphNode node = GraphNode(nodesArray[i]["id"].GetInt(),
                                     nodesArray[i]["x"].GetInt(),
                                     nodesArray[i]["y"].GetInt(),
                                     this->nodeRadius);
                    node.setLabel(nodesArray[i]["label"].GetString());

                    // Add any adjacent nodes.
                    const rapidjson::Value& adjArray = nodesArray[i]["adjacent"];
                    if (!adjArray.IsNull() && adjArray.IsArray()){
                        for (rapidjson::SizeType j = 0; j < adjArray.Size(); ++j){
                            GraphNode adj = this->findNode(adjArray[i]["nodeID"].GetInt());

                            if (adj != NULL)
                            {
                                // TODO: Fix whether undirected and weighted.
                                GraphEdge edge = GraphEdge(node, adj, true, true, false, false);
                                node.addDestinationEdge(edge);
                            }
                        }
                    }

                    this->nodes.append(node);
                }
            }
        }
    }
    else {
        std::cout << "Unable to open file" << std::endl;
    }
}

// Finds and returns the node with the given id. Returns null if not found.
GraphNode findNode(int id){

    if (this->nodes.size() > 0)
    {
        for (int i = 0; i < this->nodes.size(); ++i)
        {
            if (nodes[i].getId() == id)
            {
                return nodes[i];
            }
        }

        return NULL;
    }
}




