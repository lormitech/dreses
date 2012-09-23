
//
// Copyright (c) 2009-2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



//
// program name: Dreses
// version:      0.3.0
//             


// al-Idrisi - Dreses in Latin - (1100-1166) was a geographer and cartographer
// who lived in Sicily at the court of King Roger II.





#include "io_processing.h"

#include "dreses_common.h"
#include "geodesic_dist.h"
#include "grid.h"
#include "polygons.h"


static void usage(void);


int main(int argC, char* argV[])
{

  char* working_dir = NULL;

  //--  

  for (int i=1; i<argC; i++) {
    if (argV[i][0] == '-') {
      switch (argV[i][1]) {
        case 'h': usage();
                  return (1);
      }
    }
    else {
      working_dir = argV[i];
    }
  }

  if ((argC < 2) || (working_dir == NULL)) {
    usage();
    return (1);
  }


  //--

  // I/O specs
  IOSpecs io_specs;

  std::string workingdir(working_dir);
  io_specs.setWorkingDir(workingdir);

  // grid specs
  GridSpecs grid_specs;
  // reference point, coord. (X,Y)
  std::vector<Point> ref_points;

  std::string infilename_1("dreses_config.dat");

  InputProcess_Config in1(io_specs.getAbsolutePathName(infilename_1));
  in1.fromInput_Config(grid_specs, ref_points);

  // polygons
  std::vector<Polygon_2> polygons;

  std::string infilename_2("polygons.dat");

  InputProcess_Polygons in2(io_specs.getAbsolutePathName(infilename_2));
  in2.fromInput_Polygons(polygons);


  //--
  // timing of the process (start)
  time_t t1, t2;
  double ptime;

  (void) time(&t1);


  //--

  // evaluating the geodesic distances
  evaluateGeodesicDist(grid_specs, polygons, ref_points, io_specs);


  //--
  // timing of the process (end)
  (void) time(&t2);

  ptime = difftime(t2, t1);
  std::cout << "process time [sec.]: " << ptime << std::endl;


  return(0);

}


static void usage(void)
{
    std::cout << "\n---------------------------------------------------------------\n"
                 "                       ** Dreses **\n"
                 "                       vers. 0.3.0\n\n"
                 " Usage:\n"
                 "    $ dreses [option] <working dir. path>\n\n"
                 " Options:\n"
                 "    -h                help\n\n"
              << std::endl;

}

