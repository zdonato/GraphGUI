#ifndef ALGORITHMSLIBRARY_H
#define ALGORITHMSLIBRARY_H

#include <QLibrary>

#include "graphview.h"

typedef void (__stdcall* graphInitFunctionType)();
typedef void (__stdcall* addNodeFunctionType)(unsigned int);
typedef void (__stdcall* addUnweightedEdgeFunctionType)(unsigned int, unsigned int);
typedef void (__stdcall* addWeightedEdgeFunctionType)(unsigned int, unsigned int, unsigned int);
typedef std::vector<unsigned int> (__stdcall* sortFunctionType)();
typedef std::vector<unsigned int> (__stdcall* pathFunctionType)(unsigned int, unsigned int);

class AlgorithmsLibrary
{
  public:
    AlgorithmsLibrary();
    virtual ~AlgorithmsLibrary();

    bool load(const QString& filePath);
    void unload();
    void init(GraphView* view);
    void clearResults();

    void breadthFirstSearch();
    void depthFirstSearch();
    void topSort();
    void shortestPath(GraphNode* source, GraphNode* destination);

  private:
    GraphView* graphView;
    QLibrary* library;    
    QHash<unsigned int, GraphNode*>* nodes;
    QList<GraphEdge*>* edges;

    graphInitFunctionType createUnweightedGraphLibFunction;
    graphInitFunctionType createWeightedGraphLibFunction;

    addNodeFunctionType addNodeLibFunction;

    addUnweightedEdgeFunctionType addUnweightedDirectedEdgeLibFunction;
    addUnweightedEdgeFunctionType addUnweightedUndirectedEdgeLibFunction;

    addWeightedEdgeFunctionType addWeightedDirectedEdgeLibFunction;
    addWeightedEdgeFunctionType addWeightedUndirectedEdgeLibFunction;

    sortFunctionType breadthFirstSearchLibFunction;
    sortFunctionType depthFirstSearchLibFunction;
    sortFunctionType topSortLibFunction;

    pathFunctionType shortestPathLibFunction;
};

#endif // ALGORITHMSLIBRARY_H
