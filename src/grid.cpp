
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>
#include <CGAL/Polygon_2_algorithms.h>

#include "dreses_debugging.h"

#include "dreses_common.h"
#include "graph.h"
#include "grid.h"
#include "polygons.h"


GridSpecs::GridSpecs()
: grid_specs_(3, 0)
{

}

void GridSpecs::setGridSpecs(int nc, int nr, float gspacing)
{
  grid_specs_.clear();

  grid_specs_.push_back(nc);
  grid_specs_.push_back(nr);
  grid_specs_.push_back(gspacing);

}

std::vector<float> GridSpecs::getGridSpecs(void)
{

  return grid_specs_;

}


int getGridPointID(float x, float y, GridSpecs grid_specs)
{
  // offset
  int offset;
  offset = (int)grid_specs.getGridSpecs().at(0);

  // grid spacing
  float gspacing;
  gspacing = grid_specs.getGridSpecs().at(2);

  // grid point ID
  int gpoint_id;
  gpoint_id = (int)boost::math::round((x-1)/gspacing + 1) + (int)boost::math::round((y-1)/gspacing) * offset;


  return gpoint_id;

}


Point getCoords(int grid_point_id, GridSpecs grid_specs)
{
  // offset
  int offset;
  offset = (int)grid_specs.getGridSpecs().at(0);

  // grid spacing
  float gspacing;
  gspacing = grid_specs.getGridSpecs().at(2);

  // distance of the grid point from the left edge of the grid
  int dist_from_le;
  dist_from_le = grid_point_id - 1;
  while (dist_from_le>=offset) {
    dist_from_le = dist_from_le - offset; 
  }
  float x;
  x = (dist_from_le * gspacing) + 1;

  int scan_dist_be;
  scan_dist_be = grid_point_id;
  // distance of the grid point from the bottom edge of the grid
  int dist_from_be;
  dist_from_be = 0;
  while (scan_dist_be>offset) {
    scan_dist_be = scan_dist_be - offset;
    dist_from_be = dist_from_be + 1;
  }
  float y;
  y = (dist_from_be * gspacing) + 1;

  return(std::make_pair(x, y));

}


void projectDataToGrid(std::vector<Point_2> grid_points, std::vector<Polygon_2> polygons,
                       std::vector<Polygon_gp_id> &gpoints_id_pgn, std::vector<int> &gpoints_id_graph)
{
  // grid point ID
  // (note that the grid point at the bottom left corner is the first grid point, ID=1)
  int gp_id = 0;
  std::vector<int> gp_id_pgn_tmp;

  // status:  -1 = outside the polygon
  //           0 = on the polygon boundary
  //           1 = inside the polygon 
  CGAL::Bounded_side status;

  // note that, since the vector `grid_points' is an ordered data structure,
  // the locations of the elements within the vector give the grid point IDs
  for (int loc=0; loc<(int)grid_points.size(); ++loc) {
    status = CGAL::bounded_side_2(polygons.at(0).vertices_begin(),
                                  polygons.at(0).vertices_end(),
                                  grid_points.at(loc), K_dreses());
    gp_id = loc + 1;
    if (status != CGAL::ON_BOUNDED_SIDE) {
      gpoints_id_graph.push_back(gp_id);
    }
    else {
      gp_id_pgn_tmp.push_back(gp_id);
    }

  }

  gpoints_id_pgn.push_back(gp_id_pgn_tmp);


  //--
  #ifdef DEBUG_GRID

    std::cout << "\n>> debug - grid" << std::endl;

    std::cout << "no. of grid points associated with the first polygon: " << gpoints_id_pgn.at(0).size() << std::endl;

  #endif // DEBUG_GRID
  //--



}


void generateGridPointNames(const int no_rows, const int no_cols,
                            std::vector<std::string> &point_names)
{
  // number of grid points
  int no_gpoints;
  no_gpoints = (no_rows*no_cols);

  std::string point_name, str_digit;

  int j = 1;
  for (int i=0; i<no_gpoints; ++i) {
    str_digit = boost::lexical_cast<std::string>(j);
    point_name = "V" + str_digit;
    point_names.push_back(point_name);
    j++;
  }

}


//
// The function `evaluateROIPointsID()' evaluates the grid locations (IDs
// of the grid points) within a given ROI.
// 
// input:
//   no_roi_points  = no. of grid points within the ROI
//   offset         = no. of grid points for each row (grid width)
//   roix           = no. of grid points within the ROI in the X direction (ROI width)
//   lbound         = first grid location at the bottom left corner of the ROI
//   roi_points_id  = IDs of the grid points within a given ROI
//
//
void evaluateROIPointsID(const int no_roi_points, const int offset,
                         const int roix, const int lbound, 
                         std::vector<int> &roi_points_id)
{
  int roi_i = lbound;
  int j = 0;

  for (int i=0; i<no_roi_points; i++) {
    roi_points_id.push_back(roi_i);
    roi_i = roi_i + 1;
    j = j + 1;
    if (j == roix) {
      roi_i = roi_i + offset - roix;
      j = 0;
    }
  }


}


//
// Given a set of the data structures associated with a ROI, the function
// `evaluateRegionOfInterest()' evaluates the grid locations (IDs of the grid points).
// 
// input:
//   no_rows        = no. of rows
//   no_cols        = no. of columns
//   ref_point_id   = ID of the reference point (center of the ROI)
//   roi_r          = radius of the ROI (in grid units)
//   roi_points_id  = IDs of the grid points within a given ROI
//
void evaluateRegionOfInterest(const int no_rows, const int no_cols,
                              const int ref_point_id, const int roi_r,
                              std::vector<int> &roi_points_id)
{

  // number of grid points
  int no_gpoints;
  no_gpoints = (no_rows*no_cols);
  // offset
  int offset;
  offset = no_cols;

  // distance of the reference point from the left edge of the grid
  int dist_from_le;
  dist_from_le = ref_point_id - 1;
  while (dist_from_le>=offset) {
    dist_from_le = dist_from_le - offset; 
  }

  // distance of the reference point from the right edge of the grid
  int dist_from_re;
  dist_from_re = ref_point_id;
  while (dist_from_re>offset) {
    dist_from_re = dist_from_re - offset; 
  }
  dist_from_re = offset - dist_from_re;

  // distance of the reference point from the bottom edge of the grid
  int dist_from_be;
  dist_from_be = 0;
  int scan_dist_be;
  scan_dist_be = ref_point_id;
  while (scan_dist_be>offset) {
    scan_dist_be = scan_dist_be - offset;
    dist_from_be = dist_from_be + 1;
  }

  // distance of the reference point from the top edge of the grid
  int dist_from_te;
  dist_from_te = 0;
  int scan_dist_te;
  scan_dist_te = ref_point_id;
  while (scan_dist_te<=no_gpoints) {
    scan_dist_te = scan_dist_te + offset;
    dist_from_te = dist_from_te + 1;
  }
  dist_from_te = dist_from_te - 1;


  //--

  // first grid location at the bottom left corner of the ROI
  int lbound = 0;
  // no. of grid points within the ROI in the X direction
  int roix = 0;
  // no. of grid points within the ROI in the Y direction
  int roiy = 0;
  // no. of grid points within the ROI
  int no_roi_points = 0;

  // bottom left corner
  if ((roi_r>dist_from_le) & (roi_r>dist_from_be)) { 
    roix = dist_from_le + roi_r + 1;
    roiy = dist_from_be + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = 1;
  }

  // bottom right corner
  if ((roi_r>dist_from_re) & (roi_r>dist_from_be)) {
    roix = dist_from_re + roi_r + 1;
    roiy = dist_from_be + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = scan_dist_be - roi_r;
  }

  // top left corner
  if ((roi_r>dist_from_le) & (roi_r>dist_from_te)) {
    roix = dist_from_le + roi_r + 1;
    roiy = dist_from_te + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - dist_from_le;
  }

  // top right corner
  if ((roi_r>dist_from_re) & (roi_r>dist_from_te)) {
    roix = dist_from_re + roi_r + 1;
    roiy = dist_from_te + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - roi_r;
  }

  // bottom edge
  if ((roi_r>dist_from_be) & (roi_r<=dist_from_le) & (roi_r<=dist_from_re)) { 
    roix = roi_r + roi_r + 1;
    roiy = dist_from_be + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = scan_dist_be - roi_r;
  }

  // top edge
  if ((roi_r>dist_from_te) & (roi_r<=dist_from_le) & (roi_r<=dist_from_re)) {
    roix = roi_r + roi_r + 1;
    roiy = dist_from_te + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - roi_r;
  }

  // left edge
  if ((roi_r>dist_from_le) & (roi_r<=dist_from_be) & (roi_r<=dist_from_te)) {
    roix = dist_from_le + roi_r + 1;
    roiy = roi_r + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - dist_from_le;
  }

  // right edge
  if ((roi_r>dist_from_re) & (roi_r<=dist_from_be) & (roi_r<=dist_from_te)) {
    roix = dist_from_re + roi_r + 1;
    roiy = roi_r + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - roi_r;
  }

  // central area
  if ((roi_r<=dist_from_be) & (roi_r<=dist_from_te) &
     (roi_r<=dist_from_le) & (roi_r<=dist_from_re)) {
    roix = roi_r + roi_r + 1;
    roiy = roi_r + roi_r + 1;
    no_roi_points = roix*roiy;
    lbound = ref_point_id - roi_r*offset - roi_r;
  }


  //--
 
  evaluateROIPointsID(no_roi_points, offset, roix, lbound, roi_points_id);


}


void generateConns(const int no_rows, const int no_cols, std::vector<Conn> &conns)
{

  // number of grid points
  int no_gpoints;
  no_gpoints = (no_rows*no_cols);

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

  // connection between two grid points
  Conn conn;

  for (int gp=1; gp<=no_gpoints; gp++) {

    roi_points_id.clear();

    evaluateRegionOfInterest(no_rows, no_cols, gp, roi_r, roi_points_id);

    no_roi_points = roi_points_id.size();
    for (int rp=0; rp<no_roi_points; rp++) {
      roi_point_id = roi_points_id.at(rp);
      if (roi_point_id != gp) {
        conn = std::make_pair(gp, roi_point_id);
        conns.push_back(conn);
      }
    }
  }


}


void initGrid(const int no_rows, const int no_cols,
              std::vector<std::string> &point_names, std::vector<Conn> &conns)
{

  // generating the names of the grid points
  generateGridPointNames(no_rows, no_cols, point_names);

  // generating the connections of the grid points
  generateConns(no_rows, no_cols, conns);


}


void initGrid(GridSpecs grid_specs, std::vector<Point_2> &grid_points)
{

  // no. of rows and columns
  int no_rows, no_cols;
  no_rows = (int)grid_specs.getGridSpecs().at(0);
  no_cols = (int)grid_specs.getGridSpecs().at(1);

  float x_tmp, y_tmp;
  x_tmp = 1;
  y_tmp = 1;
  Point_2 p_tmp;

  // the grid point at the bottom left corner is the first grid point (ID=1)
  // and it is associated with the spatial location (1,1)
  for (int r=1; r<=no_rows; ++r) {
    for (int c=1; c<=no_cols; ++c) {
      p_tmp = Point_2(x_tmp, y_tmp);
      grid_points.push_back(p_tmp);
      x_tmp = x_tmp + grid_specs.getGridSpecs().at(2);
    }
    x_tmp = 1;
    y_tmp = y_tmp + grid_specs.getGridSpecs().at(2);
  }


  //--
  #ifdef DEBUG_GRID

    std::cout << "\n>> debug - grid" << std::endl;

    std::cout << "no. of grid points: " << grid_points.size() << std::endl;

  #endif // DEBUG_GRID
  //--


}

