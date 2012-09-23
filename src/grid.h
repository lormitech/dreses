
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#ifndef GRID_H
#define GRID_H


#include "polygons.h"


//
// -- grid
//
// The locations are associated with the grid points row by row, starting from the
// bottom left corner and ending to the top right corner of the grid.
//
// The grid point at the bottom left corner is the first grid point (ID=1)
// and it is associated with the spatial location (1,1).
//

typedef std::pair<float, float> Point;
typedef std::pair<int, int> Conn;


class GridSpecs{
public:
  GridSpecs();
  void setGridSpecs(int nc, int nr, float gspacing);
  std::vector<float> getGridSpecs(void);

private:
  // grid specifications: resolution [nc x nr] and grid spacing [au]
  std::vector<float> grid_specs_;

};


int getGridPointID(float x, float y, GridSpecs grid_specs);

Point getCoords(int grid_point_id, GridSpecs grid_specs);

void projectDataToGrid(std::vector<Point_2> grid_points, std::vector<Polygon_2> polygons,
                       std::vector<Polygon_gp_id> &gpoints_id_pgn, std::vector<int> &gpoints_id_graph);

void generateGridPointNames(const int no_rows, const int no_cols,
                            std::vector<std::string> &point_names);

void evaluateROIPointsID(const int no_roi_points, const int offset,
                         const int roix, const int lbound, 
                         std::vector<int> &roi_points_id);

void evaluateRegionOfInterest(const int no_rows, const int no_cols,
                              const int ref_point_id, const int roi_r,
                              std::vector<int> &roi_points_id);

void generateConns(const int no_rows, const int no_cols,
                   std::vector<Conn> &conns);

void initGrid(const int no_rows, const int no_cols,
              std::vector<std::string> &point_names, std::vector<Conn> &conns);

void initGrid(GridSpecs grid_specs, std::vector<Point_2> &grid_points);



#endif /* GRID_H */
