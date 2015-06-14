#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPainter>
#include <QPainterPath>

class GraphNode;

class GraphEdge : public QGraphicsItem
{
public:
    GraphEdge(GraphNode* source, GraphNode* destination, bool undirected, bool drawArrows, bool drawWeight, bool drawAsArc);
    virtual ~GraphEdge();

    void updateDrawing();

    int getWeight();
    void setWeight(int w);

    bool isUndirected();
    void setUndirected(bool undirected);

    void setBold(bool bold);
    void setDrawArrows(bool draw);
    void setDrawWeight(bool draw);
    void setDrawAsArc(bool drawAsArc);

    GraphNode* getSourceNode();
    GraphNode* getDestinationNode();

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void sourceUpdated();
    void destinationUpdated();
    void changeDirection();

private:
    void createArcDrawing();
    void createStraightLineDrawing();
    void cleanDrawing();

    GraphNode* source;
    GraphNode* destination;  
    QPainterPath* shapePath;
    QGraphicsTextItem* weightText;
    bool undirected;
    bool drawArrows;
    bool drawWeight;
    bool drawAsArc;
    int weight;
    bool bold;    
};

#endif // GRAPHEDGE_H
