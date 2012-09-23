
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "dreses_debugging.h"

#include "io_processing.h"

#include "dreses_common.h"
#include "graph.h"
#include "grid.h"
#include "polygons.h"


IOSpecs::IOSpecs()
: working_dir_("working_dir_path")
{

}

void IOSpecs::setWorkingDir(const std::string working_dir)
{

  working_dir_ = working_dir;

}

std::string IOSpecs::getWorkingDir(void)
{
  return working_dir_;

}

std::string IOSpecs::getAbsolutePathName(const std::string filename)
{

  return (working_dir_ + filename);

}


//--

InputProcess_Config::InputProcess_Config(std::string input_file)
: fs_in_c_(input_file.c_str())
{

}

void InputProcess_Config::fromInput_Config(GridSpecs &grid_specs, std::vector<Point> &ref_points)
{
  int i = 0;
  int no_lines = 0;
  float param;
  float* params = new float[no_params_7];
  std::string line;


  if (!fs_in_c_) {
    std::cerr << "\n> error - input file opening (dreses_config.dat)" << std::endl;
    exit(1);
  }

  while(getline(fs_in_c_, line, '\n')){

    if (line.size() == 0 || line[0] == '#') continue;

    char* start;
    char* end;
    const char* end_t;
    
    end_t = line.c_str();
    end = const_cast<char*>(end_t);
    do {
      if (i == no_params_7) break;
      start = end;
      param = (float)strtod(start, &end);
      params[i] = param;
      i++;
    } while ((end != NULL) && (end != start));

    i--;
    no_lines++;

  }

  fs_in_c_.close();


  grid_specs.setGridSpecs(params[0], params[1], params[2]);

  ref_points.push_back(std::make_pair(params[3], params[4]));
  ref_points.push_back(std::make_pair(params[5], params[6]));


  delete [] params;


  //--
  #ifdef DEBUG_IO

    std::cout << "\n>> debug - I/O processing (dreses_config.dat)" << std::endl;

    std::cout << "grid specs (nc, nr, grid spacing): " << grid_specs.getGridSpecs().at(0) << " "
                                                       << grid_specs.getGridSpecs().at(1) << " "
                                                       << grid_specs.getGridSpecs().at(2) << " " << std::endl;
    std::cout << "reference points: " << ref_points.at(0).first << " " << ref_points.at(0).second << ", "
                                      << ref_points.at(1).first << " " << ref_points.at(1).second << std::endl;

  #endif // DEBUG_IO
  //--


}


//--

InputProcess_Polygons::InputProcess_Polygons(std::string input_file)
: fs_in_p_(input_file.c_str())
{

}

void InputProcess_Polygons::fromInput_Polygons(std::vector<Polygon_2> &polygons)
{
 
  std::string line;
  std::vector<std::string> sstrings_l;
  std::vector<float> values_l;
  std::vector<std::vector<float> > values_t;


  if (!fs_in_p_) {
    std::cerr << "\n> error - input file opening (polygons.dat)" << std::endl;
    exit(1);
  }

  while (getline(fs_in_p_, line, '\n')) {

    if (line.size() == 0 || line[0] == '#') continue;

    splitString(line, ' ', sstrings_l);

    for (int i=0; i<(int)sstrings_l.size(); ++i){
      values_l.push_back(boost::lexical_cast<float>(sstrings_l[i]));
    }
    values_t.push_back(values_l);
    sstrings_l.clear();
    values_l.clear();
  
  }

  fs_in_p_.close();


  float x_tmp, y_tmp;
  Point_2 p_temp;
  std::vector<Point_2> ds_p_temp;
  Polygon_2 pgn_tmp;

  std::vector<std::vector<float> >::const_iterator ic1;
  vi_cgal vi;
 
  for (ic1=values_t.begin(); ic1!=values_t.end(); ++ic1) {

    for (int j=0; j<(int)(*ic1).size(); ++j) {
      x_tmp = (*ic1).at(j);
      j++;
      y_tmp = (*ic1).at(j);
      p_temp = Point_2(x_tmp, y_tmp);
      ds_p_temp.push_back(p_temp);
    }
    vi = pgn_tmp.vertices_end();
    pgn_tmp.insert(vi, ds_p_temp.begin(), ds_p_temp.end());
    polygons.push_back(pgn_tmp);
    ds_p_temp.clear();
    pgn_tmp.clear();

  }


  //--
  #ifdef DEBUG_IO

    std::cout << "\n>> debug - I/O processing (polygons.dat)" << std::endl;

    std::cout << "no. of polygons: " << polygons.size() << std::endl;
    for (int w=0; w<(int)polygons.size(); ++w) {
      std::cout << "polygon " << (w+1) << " - ";
      printPolygon(polygons.at(w));
    }

  #endif // DEBUG_IO
  //--


}

void splitString(const std::string &string, char delimiter, std::vector<std::string> &substrings)
{

  std::string::size_type i = 0;
  std::string::size_type j = string.find(delimiter);

  while (j != std::string::npos) {

    substrings.push_back(string.substr(i, j-i));
    i = ++j;
    j = string.find(delimiter, j);

    if (j == std::string::npos) {
      substrings.push_back(string.substr(i, string.length()));
    }

  }

}



//--

OutputProcess_Dreses::OutputProcess_Dreses(std::string output_file)
: fs_out_g_(output_file.c_str())
{


}

void OutputProcess_Dreses::toOutput(std::vector<Point> ref_points, std::vector<Polygon_2> polygons,
                                    GridSpecs grid_specs,
                                    int no_gpoints_png, int no_gpoints_graph)
{
  // start and end points
  int spoint_id, epoint_id;
  spoint_id = getGridPointID(ref_points.at(0).first, ref_points.at(0).second, grid_specs);
  epoint_id = getGridPointID(ref_points.at(1).first, ref_points.at(1).second, grid_specs);
  std::string spoint_node, epoint_node;
  spoint_node = "V" + boost::lexical_cast<std::string>(spoint_id);
  epoint_node = "V" + boost::lexical_cast<std::string>(epoint_id);


  fs_out_g_ << "\n       ** Dreses - summary **\n\n" << std::endl;

  fs_out_g_ << "start point - coordinates: " << ref_points.at(0).first << " " << ref_points.at(0).second
            << "; grid point ID: " << spoint_id
            << "; node name: " << spoint_node << std::endl;
  fs_out_g_ << "end point   - coordinates: " << ref_points.at(1).first << " " << ref_points.at(1).second
            << "; grid point ID: " << epoint_id
            << "; node name: " << epoint_node << "\n" << std::endl;

  fs_out_g_ << "no. of polygons: " << polygons.size() << std::endl;
  fs_out_g_ << "the polygon is "
            << (polygons.at(0).is_simple() ? "" : "not ") << "simple" << std::endl;
  fs_out_g_ << "the polygon is "
            << (polygons.at(0).is_convex() ? "" : "not ") << "convex" << "\n" << std::endl;

  fs_out_g_ << "grid specs (nc, nr, grid spacing): " << grid_specs.getGridSpecs().at(0) << " "
                                                     << grid_specs.getGridSpecs().at(1) << " "
                                                     << grid_specs.getGridSpecs().at(2) << " " << std::endl;
  fs_out_g_ << "no. of grid points associated with the polygon(s): " << no_gpoints_png << std::endl;
  fs_out_g_ << "no. of grid points associated with the graph: " << no_gpoints_graph << std::endl;

}

OutputProcess_Distances::OutputProcess_Distances(std::string output_file)
: fs_out_d_(output_file.c_str())
{


}

void OutputProcess_Distances::toOutput(GridSpecs grid_specs, int no_gpoints, int s_node_id,
                                       std::vector<std::string> node_names, vertices_size_type* distances)
{

  int i_e = 0;
  for (int i=0; i<no_gpoints; i++) {
    if ((distances[i]!=0) || (distances[i]==0 && i==s_node_id)) {
      fs_out_d_ << node_names.at(i_e) << ": D = " << distances[i]*grid_specs.getGridSpecs().at(2) << std::endl;
      i_e++;
    }
  }


}
