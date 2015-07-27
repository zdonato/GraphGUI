#include "algorithmslibrary.h"

AlgorithmsLibrary::AlgorithmsLibrary() : graphView(NULL), library(NULL), nodes(NULL), edges(NULL)
{
    nodes = new QHash<unsigned int, GraphNode*>();
    edges = new QList<GraphEdge*>();
}

AlgorithmsLibrary::~AlgorithmsLibrary()
{
    unload();
    delete library;
}

void AlgorithmsLibrary::unload()
{
    delete nodes;
    nodes = NULL;
    delete edges;
    edges = NULL;

    if (library)
        library->unload();
}

void AlgorithmsLibrary::clearResults()
{
    QHashIterator<unsigned int, GraphNode*> nodesIt(*nodes);
    while (nodesIt.hasNext()) {
        nodesIt.next();
        GraphNode* node = nodesIt.value();
        node->setSubscript("");
    }

    QListIterator<GraphEdge*> edgesIt(*edges);
    while (edgesIt.hasNext()) {
        GraphEdge* edge = edgesIt.next();
        edge->setBold(false);
    }

    if (graphView)
        graphView->scene()->update();
}

bool AlgorithmsLibrary::load(const QString& filePath)
{
    if (library && library->isLoaded()) {
        unload();
        library->setFileName(filePath);
        library->load();
    }
    else
        library = new QLibrary(filePath);

    createUnweightedGraphLibFunction = (graphInitFunctionType)library->resolve("createUnweightedGraph");
    if (!createUnweightedGraphLibFunction)
        return false;

    createWeightedGraphLibFunction = (graphInitFunctionType)library->resolve("createWeightedGraph");
    if (!createWeightedGraphLibFunction)
        return false;

    addNodeLibFunction = (addNodeFunctionType)library->resolve("addNode");
    if (!addNodeLibFunction)
        return false;

    addUnweightedDirectedEdgeLibFunction = (addUnweightedEdgeFunctionType)library->resolve("addUnweightedDirectedEdge");
    if (!addUnweightedDirectedEdgeLibFunction)
        return false;

    addUnweightedUndirectedEdgeLibFunction = (addUnweightedEdgeFunctionType)library->resolve("addUnweightedUndirectedEdge");
    if (!addUnweightedUndirectedEdgeLibFunction)
        return false;

    addWeightedDirectedEdgeLibFunction = (addWeightedEdgeFunctionType)library->resolve("addWeightedDirectedEdge");
    if (!addWeightedDirectedEdgeLibFunction)
        return false;

    addWeightedUndirectedEdgeLibFunction = (addWeightedEdgeFunctionType)library->resolve("addWeightedUndirectedEdge");
    if (!addWeightedUndirectedEdgeLibFunction)
        return false;

    breadthFirstSearchLibFunction = (sortFunctionType)library->resolve("breadthFirstSearch");
    if (!breadthFirstSearchLibFunction)
        return false;

    depthFirstSearchLibFunction = (sortFunctionType)library->resolve("depthFirstSearch");
    if (!depthFirstSearchLibFunction)
        return false;

    topSortLibFunction = (sortFunctionType)library->resolve("topSort");
    if (!topSortLibFunction)
        return false;

    shortestPathLibFunction = (pathFunctionType)library->resolve("shortestPath");
    if (!shortestPathLibFunction)
        return false;

    return true;
}

void AlgorithmsLibrary::init(GraphView* view)
{
    graphView = view;
    nodes->clear();
    edges->clear();

    if (view->isGraphWeighted())
        createWeightedGraphLibFunction();
    else
        createUnweightedGraphLibFunction();

    QListIterator<GraphNode*> nodesIt(view->getNodes());
    while (nodesIt.hasNext()) {
        GraphNode* node = nodesIt.next();
        nodes->insert(node->getId(), node);
        addNodeLibFunction(node->getId());
    }

    QListIterator<GraphEdge*> edgesIt(view->getEdges());
    while (edgesIt.hasNext()) {
        GraphEdge* edge = edgesIt.next();
        edges->push_back(edge);

        unsigned int srcId = edge->getSourceNode()->getId();
        unsigned int dstId = edge->getDestinationNode()->getId();
        unsigned int weight = edge->getWeight();

        if (edge->isUndirected()) {
            if (view->isGraphWeighted())
                addWeightedUndirectedEdgeLibFunction(srcId, dstId, weight);
            else
                addUnweightedUndirectedEdgeLibFunction(srcId, dstId);
        }
        else {
            if (view->isGraphWeighted())
                addWeightedDirectedEdgeLibFunction(srcId, dstId, weight);
            else
                addUnweightedDirectedEdgeLibFunction(srcId, dstId);
        }
    }
}

void AlgorithmsLibrary::breadthFirstSearch()
{
    std::vector<unsigned int> result = breadthFirstSearchLibFunction();

    for (unsigned int i = 0; i < result.size(); ++i)
        (*nodes)[result[i]]->setSubscript(QString::number(i+1));

    graphView->scene()->update();
}

void AlgorithmsLibrary::depthFirstSearch()
{
    std::vector<unsigned int> result = depthFirstSearchLibFunction();

    for (unsigned int i = 0; i < result.size(); ++i)
        (*nodes)[result[i]]->setSubscript(QString::number(i+1));

    graphView->scene()->update();
}

void AlgorithmsLibrary::topSort()
{
    std::vector<unsigned int> result = topSortLibFunction();

    for (unsigned int i = 0; i < result.size(); ++i)
        (*nodes)[result[i]]->setSubscript(QString::number(i+1));

    graphView->scene()->update();
}

void AlgorithmsLibrary::shortestPath(GraphNode* source, GraphNode* destination)
{
    unsigned int srcId = source->getId();
    unsigned int dstId = destination->getId();
    std::vector<unsigned int> result = shortestPathLibFunction(srcId, dstId);

    for (unsigned int i = 0; i < result.size() ; ++i) {
        GraphNode* nodeSrc = (*nodes)[result[i]];
        nodeSrc->setSubscript(QString::number(i+1));

        if (i < result.size() - 1) {
            GraphNode* nodeDst = (*nodes)[result[i+1]];

            QListIterator<GraphEdge*> edgesIt(nodeSrc->getSourceEdges());
            while (edgesIt.hasNext()) {
                GraphEdge* edge = edgesIt.next();
                if (edge->getDestinationNode() == nodeDst || edge->getSourceNode() == nodeDst) {
                    edge->setBold(true);
                    break;
                }
            }
        }
    }

    graphView->scene()->update();
}
