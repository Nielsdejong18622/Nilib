#ifndef _NODE_H
#define _NODE_H
#pragma once
#include "Dispch.h"

/* 
Graph class allowance. 
-------------------------------------------------------------------------------------------------------
Graph class | Nodes connect by arc | parallel edges | Self-loops | Bipartite | DisConnected | Acyclic
Hypergraph  | 2>=                       yes               yes         yes          yes        yes
Multigraph  | 2                         yes               yes         yes          yes        yes
Graph       | 2                         no                yes         yes          yes        yes
Simplegraph | 2                         no                no          yes          yes        yes
Forest      | 2                         no                no          yes          yes        no 
Tree        | 2                         no                no          yes          no         no 
*/


namespace {

// Node container class. 
struct Node
{
    double x = 0, y = 0;
    size_t color, weigth, size = 0;
    char *label = "";
    void *ptr = nullptr;
};

// Arc container class. 
struct Arc
{
    size_t color, capacity, size = 0;
    char *label = "";
    void *ptr = nullptr;
};

// Same meaning, same functionality. 
// Vertices exist in Graphics. 
typedef Node Vertex;

class Matrix;

// Directed hypergraph. Self-loops and double arcs permitted. Adjacency list implementation.  
// Every node is refered with an internal index (order of adding the node to the graph).
// Node const &DirectedHyperGraph.node(3) then returns node 3 (struct form). 
// Similarly, size_t DirectedHyperGraph.degree(3) then returns the degree of node 3. 
class DirectedHyperGraph
{
public:
    DirectedHyperGraph();
    DirectedHyperGraph(std::vector<Node> const &nodes, std::vector<Arc> const &arcs);

    // Add and remove node(s) and arc(s). 
    void add(Node const &node); // O(1) complexity
    void add(std::vector<Node> const &nodes); // O(1) complexity

    void add(Arc const &arc, std::vector<Node> const &from, std::vector<Node> const &to);
    void add(std::vector<Arc> const &arcs, std::vector<std::vector<Node>> const &from, std::vector<std::vector<Node>> const &to); 

    void remove(Node const &node); // O(n) complexity
    void remove(std::vector<Node> const &nodes); // O(n) complexity 

    void remove(Arc const &node, Node const &from, Node const &to); // O(n*e) complexity 
    void remove(std::vector<Arc> const &arcs, std::vector<Node> const &from, std::vector<Arc> const &to); // O(n*e) complexity 

    // Information about the graph. 
    bool const weaklyconnected() const;
    bool const stronglyconnected() const;
    bool const empty() const;
    // bool const tree() const;
    // bool const acyclic() const;
    
    // Number of nodes
    size_t const order() const;
    // Number of arcs. 
    double const size() const;  
    
    // Node specific information.
    size_t const indegree(Node) const;
    size_t const outdegree(Node) const;
    size_t const degree(Node n) const { return indegree(n) + outdegree(n); };


    std::vector<Arc> forwardStar(Node) const; // Incoming edges.
    std::vector<Arc> backwardStar(Node) const; // Outgoing edges.
    
    std::vector<Node> adjacent(Node) const;
    std::vector<Arc> incident(Node) const;
    std::vector<Node> const nodes() const;


    // Arc specific information.
    bool const incident(Arc Arc1, Arc Arc2) const; // True if arcs share same vertex. 
    std::vector<Arc> const arcs() const;
    

    // Node-Node information. 
    double const shortestdist(Node Node1, Node Node2) const;
    std::vector<Node> shortestpath(Node Node1, Node Node2) const;
    bool const adjacent(Node Node1, Node Node2) const; // True if the nodes are connected by arc. 

    // Arc-Arc information. 
    bool const adjacent(Arc Arc1, Arc Arc2) const; // True if the nodes are connected by arc. 


    // Different representations. 
    Matrix const adjacencyMatrix() const; // Return a adjacency matrix representation. 
    Matrix const laplacian() const; // Return the laplacian matrix representation. 
    
    // Comparisons
    bool const isomorphic(DirectedHyperGraph const &other) const; // Determine iso-morphism in structure.
    bool const isomorphic(DirectedHyperGraph const &&other) const; 

    bool const operator==(DirectedHyperGraph const digraph) const;  // Equality. Requires iso-morphism. 
    
    // Delete these notion.  
    bool const operator<(DirectedHyperGraph const digraph) = delete; 
    bool const operator>(DirectedHyperGraph const digraph) = delete; 
    bool const operator<=(DirectedHyperGraph const digraph) = delete; 
    bool const operator<=(DirectedHyperGraph const digraph) = delete; 

    // Other operators. 
    //friend DirectedMultiGraph operator+()
    //friend DirectedMultiGraph operator-() 


protected: // Accessible by derived classes.  
    std::map<Node, std::vector<Arc>> d_adjlist;

private: // Inaccessible by derived classes. 

};
class UndirectedHyperGraph : protected DirectedHyperGraph
{
};
typedef UndirectedHyperGraph HyperGraph;

// Example inheritance: A simple hypergraph is a graph where none of the edges are a subset of another. 
class SimpleHyperGraph : protected UndirectedHyperGraph
{
    
};

// Parallel arcs exists between nodes. Only two nodes per arc. 
class DirectedMultiGraph : protected DirectedHyperGraph
{
public:

    void add(Arc const &arc, Node const &from, Node const &to); // Add this

    // Override these
    void add(std::vector<Arc> const &arcs, std::vector<std::vector<Node>> const &from, std::vector<std::vector<Node>> const &to)
    {
        // Loop over the set of arcs and add them.
        // TODO: Naive implementation, can be much faster. 
        for (size_t arcidx = 0; arcidx < arcs.size(); ++arcidx)
        {
            add(arcs[arcidx], from[arcidx], to[arcidx]);
        }
    };
    void add(Arc const &arc, std::vector<Node> const &from, std::vector<Node> const &to)
    {
        if (to.size() != 1 or from.size() != 1)
            throw std::invalid_argument("Attempting to add hyper edge/arc where non-hyper was expected.");
        add(arc, from[0], to[0]);
    }; 
};
class UndirectedMultiGraph : protected DirectedMultiGraph
{
public:
    void add(Arc const &arc, std::vector<Node> const &from, std::vector<Node> const &to)
    {
        DirectedMultiGraph::add(arc, from, to);
        DirectedMultiGraph::add(arc, to, from);
    }; 
};
typedef UndirectedMultiGraph Multigraph;
// No parallel arcs. 
class DirectedGraph : protected DirectedMultiGraph
{

};
class UndirectedGraph : protected DirectedGraph
{

};
typedef UndirectedGraph Graph;
typedef DirectedGraph Digraph;

// No self-loops 
class DirectedSimpleGraph : protected DirectedGraph
{

};
class UndirectedSimpleGraph : protected DirectedSimpleGraph
{

};
typedef UndirectedSimpleGraph SimpleGraph;
typedef SimpleGraph StrictGraph;

// Every node is either red or blue and every edge connects red with blue.
class DirectedBipartiteGraph : protected DirectedSimpleGraph
{
    bool const balanced() const; // Are there as many red as blue nodes?
};
class UndirectedBipartiteGraph : protected DirectedBipartiteGraph
{
    bool const balanced() const; // Are there as many red as blue nodes?
};
typedef UndirectedBipartiteGraph BipartiteGraph;

// Disconnected bipartite graphs. 
class DirectedForest : protected DirectedBipartiteGraph
{

};
class UndirectedForest : protected DirectedForest
{

};
typedef UndirectedForest Forest;

// Connected bipartite graph. 
class DirectedTree : protected DirectedForest
{

};
class UndirectedTree : protected DirectedTree
{

};
typedef UndirectedTree Tree;


// Friends
// linegraph.  Transposed graph. 


// Construct random graphs. 

// Create random graph layout.

// Create smarter graph layout. 

} // Close namespace.
#endif



///@brief examples. 

// Self Organizing map. 
class SOM : protected DirectedGraph
{

};

class NeuralNetwork : protected Graph
{

};

class IntegerLinearProgramTree : protected Tree
{

};

class VRPinstance : protected SimpleGraph
{

};