
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#ifndef IO_PROCESSING_H
#define IO_PROCESSING_H


#include <fstream>

#include "graph.h"
#include "grid.h"
#include "polygons.h"


class GridSpecs;


const int no_params_7 = 7;


// I/O specifications
class IOSpecs{
public:
  IOSpecs();
  void setWorkingDir(const std::string working_dir);
  std::string getWorkingDir(void);
  std::string getAbsolutePathName(const std::string filename);

private:
  // working directory path
  std::string working_dir_;

};


// input file `dreses_config.dat'
//
// the input file consists of 2 lines (excluding comments and blank lines) associated with:
// - grid specifications
// - coordinates of the reference points (start point and end point)
//
class InputProcess_Config{
public:
  InputProcess_Config(std::string input_file);
  void fromInput_Config(GridSpecs &grid_specs, std::vector<Point> &ref_points);

private:
  std::fstream fs_in_c_;

};


// input file `polygons.dat'
//
// input:
// - coordinates (X,Y) of the vertices of the polygon(s)
//
class InputProcess_Polygons{
public:
  InputProcess_Polygons(std::string input_file);
  void fromInput_Polygons(std::vector<Polygon_2> &polygons);

private:
  std::fstream fs_in_p_;

};



//--

// output file `Dreses.dat'
//
// ouput:
// - system details (reference points, polygons, grid, ...)
//
class OutputProcess_Dreses{
public:
  OutputProcess_Dreses(std::string output_file);
  void toOutput(std::vector<Point> ref_points, std::vector<Polygon_2> polygons,
                GridSpecs grid_specs,
                int no_gpoints_png, int no_gpoints_graph);

private:
  std::ofstream fs_out_g_;

};


// output file `distances.dat'
//
// ouput:
// - distances from the source to each node
//
class OutputProcess_Distances{
public:
  OutputProcess_Distances(std::string output_file);
  void toOutput(GridSpecs grid_specs, int no_gpoints, int s_node_id,
                std::vector<std::string> node_names, vertices_size_type* distances);

private:
  std::ofstream fs_out_d_;

};



//--

// function for parsing
void splitString(const std::string &string, char delimiter, std::vector<std::string> &substrings);


template<class Kernel, class Container>
void printPolygon (const CGAL::Polygon_2<Kernel, Container>& P)
{
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  vic_cgal;

  std::cout << P.size() << " vertices: ";
  for (vic_cgal=P.vertices_begin(); vic_cgal!=P.vertices_end(); ++vic_cgal) {
    std::cout << "(" << *vic_cgal << ") ";
  }
  std::cout << std::endl;

}


#endif /* IO_PROCESSING_H */
