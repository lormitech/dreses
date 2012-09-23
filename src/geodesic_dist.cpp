
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#include <algorithm>
#include <utility>
#include <string>
#include <vector>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

#include "dreses_debugging.h"

#include "io_processing.h"

#include "dreses_common.h"
#include "graph.h"
#include "grid.h"
#include "polygons.h"


void evaluateGeodesicDist(GridSpecs grid_specs, std::vector<Polygon_2> polygons,
                          std::vector<Point> ref_points, IOSpecs io_specs)
{

  // -- grid definition --
 
  // no. of rows and columns
  int no_rows, no_cols;
  no_rows = (int)grid_specs.getGridSpecs().at(0);
  no_cols = (int)grid_specs.getGridSpecs().at(1);

  // grid points
  int no_gpoints;
  no_gpoints = (no_rows*no_cols);

  // spatial locations of the grid points
  std::vector<Point_2> grid_points;


  // evaluating the spatial locations of the grid points
  initGrid(grid_specs, grid_points);



  //--

  // -- graph definition --

  //
  // note that two different graphs need to be considered:
  //
  // 1) effective graph -
  //    graph associated with the complementary domain;
  //    the nodes of this graph do not include the grid points
  //    associated with the polygon(s);
  //
  // 2) BGL graph -
  //    graph processed by the BGL library;
  //    this graph coincides with the grid;
  //    the nodes of this graph coincide the grid points;
  //    the nodes associated with the polygon(s) are nodes
  //    with no edges, i.e. they are nodes of degree zero;
  //
  //
  //  no. of nodes (effective graph) < no. of nodes (BGL graph)
  //  no. of edges (effective graph) = no. of edges (BGL graph)
  //


  // grid points IDs associated with the polygon(s)
  std::vector<Polygon_gp_id> gpoints_id_pgn;
  // grid points IDs associated with the graph
  std::vector<int> gpoints_id_graph;


  // project the data to the grid;
  // evaluate the grid points IDs associated with the polygon(s) and
  // those associated with the complementary domain (graph)
  projectDataToGrid(grid_points, polygons, gpoints_id_pgn, gpoints_id_graph);


  // no. of grid points associated with the polygon
  int no_gpoints_png = gpoints_id_pgn.at(0).size();
  // no. of grid points associated with the graph
  int no_gpoints_graph = gpoints_id_graph.size();

  // writing output file `Dreses.dat'
  std::string outfile_name_1("Dreses.dat");

  OutputProcess_Dreses out1(io_specs.getAbsolutePathName(outfile_name_1));
  out1.toOutput(ref_points, polygons,
                grid_specs, no_gpoints_png, no_gpoints_graph);


  grid_points.clear();
  polygons.clear();

  
  // names of the nodes (vertices)
  std::vector<std::string> node_names;
  // edges
  std::vector<Edge> edges;

  // evaluating the node names and the edges of the `effective graph'
  initGraph(grid_specs, gpoints_id_pgn, gpoints_id_graph, node_names, edges);


  gpoints_id_pgn.clear();
  gpoints_id_graph.clear();


  // number of edges
  int no_edges;
  no_edges = edges.size();

  // weights
  float* weights = new float[no_edges];

  for (int i=0; i<no_edges; i++) {
    weights[i] = grid_specs.getGridSpecs().at(2);
  }


  // BGL graph
  GridGraph g(edges.begin(), edges.end(), weights, no_gpoints);
  GridGraph g_copy(edges.begin(), edges.end(), weights, no_gpoints);


  //--
  #ifdef DEBUG_GRAPH

    std::cout << "\n>> debug - graph" << std::endl;

    std::cout << "no. of nodes (effective graph): " << node_names.size() << std::endl;
    std::cout << "no. of nodes (BGL graph): " << no_gpoints << std::endl;
    std::cout << "no. of edges: " << edges.size() << std::endl;
    write_graphviz(std::cout, g);

  #endif // DEBUG_GRAPH
  //--


  // parents of the nodes
  std::vector<vertex_descriptor> parents(num_vertices(g));

  // source node ID
  // note that node ID = (grid point ID - 1)
  int s_node_id;
  s_node_id = (getGridPointID(ref_points.at(0).first, ref_points.at(0).second, grid_specs) - 1);

  // source node
  vertex_descriptor s_node = vertex(s_node_id, g);
  // the parent of `s_node' is `s_node' (distance = 0)
  parents[s_node] = s_node;

  // distances from the source to each node
  // (including the null distances - i.e. the distance from the source to itself
  // and the distances from the source to the nodes of degree zero)
  vertices_size_type distances[no_gpoints];
  std::fill_n(distances, no_gpoints, 0);

  // evaluating the distances by using the Breadth-First Search (BFS) algorithm
  breadth_first_search(g,
                       s_node,
                       visitor(make_bfs_visitor(std::make_pair(record_distances(distances,
                                                                                on_tree_edge()),
                                                               std::make_pair(record_predecessors(&parents[0],
                                                                                                  on_tree_edge()),
                                                                              copy_graph(g_copy,
                                                                                         on_examine_edge())
                                                                             )
                                                              )
                                               )
                              )
                      );


  // writing output file `distances.dat'
  std::string outfile_name_2("distances.dat");

  OutputProcess_Distances out2(io_specs.getAbsolutePathName(outfile_name_2));
  out2.toOutput(grid_specs, no_gpoints, s_node_id, node_names, distances);


  delete [] weights;


}

