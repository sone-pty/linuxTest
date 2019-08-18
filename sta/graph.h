#ifndef GRAPH_H
#define GRAPH_H

#include "std.h"
using namespace std;

template <typename Tv,typename Te> class Graph
{
private:
    void reset();
public:
    int n;
    int e;
};

typedef enum {UNDISCOVERED,DISCOVERED,VISITED} VStatus;
template <typename v> struct Vertex
{
    v data;
    int outDegree;
    int inDegree;
    VStatus status;
    int dtime;
    int ftime;
    int parent;
    int priority;
    Vertex(v value)
    {
        data = value;
        outDegree = 0;
        inDegree = 0;
        status = UNDISCOVERED;
        dtime = -1;
        ftime = -1;
        parent = -1;
        priority = INT32_MAX;
    }
};

typedef enum {UNDETERMINED,TREE,CROSS,FORWORD,BACKWARD} EStatus;
template <typename v> struct Edge
{
    v data;
    int weight;
    EStatus status;
    Edge(v value,int w)
    {
        data = value;
        weight = w;
        status = UNDETERMINED;
    }
};

template <typename Tv,typename Te> class GraphMatrix : public Graph<Tv,Te>
{
private:
    vector<Vertex<Tv>> V;
    vector<vector<Edge<Te>*>> E;
public:
    GraphMatrix(){}
    virtual ~GraphMatrix()
    {
        for(int i = 0;i < this->n;i++)
            for(int j = 0;j < this->n;j++)
                delete E[i][j];
    }

    virtual bool exists (int i, int j)
    {
        return (i >= 0 && i < this->n && j >= 0 && j < this->n && E[i][j] != NULL);
    }

    virtual void insert ( Te const& edge, int w, int i, int j )
    {
        if(E[i][j] != NULL)
            return;
        E[i][j] = new Edge<Te>(edge,w);
        this->e++;
        V[i].outDegree++;
        V[j].inDegree++;
    }

    virtual Te remove ( int i, int j )
    {
        Te oldValue = E[i][j]->data;
        delete E[i][j];
        E[i][j] = NULL;
        this->e--;V[i].outDegree--;V[j].inDegree--;
        return oldValue;
    }
};
#endif // GRAPH_H
