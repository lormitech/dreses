
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#include <algorithm>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "dreses_common.h"
#include "graph.h"
#include "grid.h"
#include "polygons.h"



void generateEdges(GridSpecs grid_specs, std::vector<int> gpoints_id_graph,
                   std::vector<Polygon_gp_id> gpoints_id_pgn, std::vector<Edge> &edges)
{

  // no. of rows and columns
  int no_rows, no_cols;
  no_rows = (int)grid_specs.getGridSpecs().at(0);
  no_cols = (int)grid_specs.getGridSpecs().at(1);

  // number of grid points
  int no_points;
  no_points = (no_rows*no_cols);

  // radius of the ROI
  int roi_r;
  roi_r = 1;
  // ID of a grid point within a given ROI
  // (note that the grid point at the bottom left corner is the first grid point, ID=1)
  int roi_point_id;
  // IDs of the grid points within a given ROI
  std::vector<int> roi_points_id;
  // no. of the grid points within a given ROI
  int no_roi_points;

  // edge between two nodes
  Edge edge;

  std::vector<int>::iterator result;

  for (int i=0; i<(int)gpoints_id_graph.size(); i++) {

    roi_points_id.clear();

    evaluateRegionOfInterest(no_rows, no_cols, gpoints_id_graph.at(i), roi_r, roi_points_id);

    no_roi_points = roi_points_id.size();
    for (int rp=0; rp<no_roi_points; rp++) {
      roi_point_id = roi_points_id.at(rp);
      // checking if the grid point ID within the ROI belongs to the
      // set of the grid point IDs associated with the polygon
      result = find(gpoints_id_pgn.at(0).begin(), gpoints_id_pgn.at(0).end(), roi_point_id);
      if (result == gpoints_id_pgn.at(0).end()) {
        if (roi_point_id != gpoints_id_graph.at(i)) {
          // note that the grid and the graph follow different conventions:
          // the first grid point has ID=1, the first node of the graph has ID=0
          edge = std::make_pair((gpoints_id_graph.at(i)-1), (roi_point_id-1));
          edges.push_back(edge);
        }
      }
    }

  }


}


void generateNodeNames(std::vector<int> gpoints_id_graph, std::vector<std::string> &node_names)
{

  std::string node_name, str_digit;

  for(int i=0; i<(int)gpoints_id_graph.size(); ++i){
    str_digit = boost::lexical_cast<std::string>(gpoints_id_graph.at(i));
    node_name = "V" + str_digit;
    node_names.push_back(node_name);
  }

}


void initGraph(GridSpecs grid_specs,
               std::vector<Polygon_gp_id> gpoints_id_pgn, std::vector<int> gpoints_id_graph,
               std::vector<std::string> &node_names, std::vector<Edge> &edges)
{

  // generating the names of the grid nodes
  generateNodeNames(gpoints_id_graph, node_names);

  // generating the grid edges
  generateEdges(grid_specs, gpoints_id_graph, gpoints_id_pgn, edges);


}

