
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#ifndef GRAPH_H
#define GRAPH_H


// Boost Graph Library (BGL)
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include "polygons.h"


using namespace boost;

class GridSpecs;


//
// -- graph associated with the grid
//
// The grid point at the bottom left corner is the first grid point (ID=1)
// and it is associated with the spatial location (1,1).
//
// The graph node at the bottom left corner is the first node of the graph (ID=0)
// and it is associated with the spatial location (1,1).
//
// (note that the grid and the graph follow different conventions)
//

// Graph Data Structure - the adjacency-list representation is
// the best choice for a `sparse' graph
typedef property<vertex_name_t, std::string> VertexP;
typedef property<edge_weight_t, int> EdgeP;
typedef adjacency_list<vecS, vecS, directedS, VertexP, EdgeP, no_property, listS> GridGraph;

typedef graph_traits <GridGraph>::vertex_descriptor vertex_descriptor;
typedef graph_traits <GridGraph>::vertex_iterator vertex_iterator;
typedef graph_traits<GridGraph>::vertices_size_type vertices_size_type;
typedef graph_traits <GridGraph>::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;


template <class NewGraph, class Tag>
struct graph_copier : public base_visitor< graph_copier<NewGraph, Tag> > {
  typedef Tag event_filter;

  graph_copier(NewGraph& graph) : new_g(graph) { }

  template <class Edge, class Graph>
  void operator()(Edge e, Graph& g) {
    add_edge(source(e, g), target(e, g), new_g);
  }

private:
  NewGraph& new_g;

};

template <class NewGraph, class Tag>
inline graph_copier<NewGraph, Tag>
copy_graph(NewGraph& g, Tag) {
  return graph_copier<NewGraph, Tag>(g);
}


void generateEdges(GridSpecs grid_specs, std::vector<int> gpoints_id_graph,
                   std::vector<Polygon_gp_id> gpoints_id_pgn, std::vector<Edge> &edges);

void generateNodeNames(std::vector<int> gpoints_id_graph, std::vector<std::string> &node_names);

// function to initialize the `effective graph'
void initGraph(GridSpecs grid_specs,
               std::vector<Polygon_gp_id> gpoints_id_pgn, std::vector<int> gpoints_id_graph,
               std::vector<std::string> &node_names, std::vector<Edge> &edges);



#endif /* GRAPH_H */
