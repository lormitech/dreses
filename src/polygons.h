
//
// Copyright (c) 2010
// Author: Lorenzo Milazzo
//
// All rights reserved.
//



#ifndef POLYGONS_H
#define POLYGONS_H


// Computational Geometry Algorithms Library (CGAL)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>


//
// -- polygons
//

// definition of the `kernel'
typedef CGAL::Exact_predicates_inexact_constructions_kernel K_dreses;

typedef K_dreses::Point_2 Point_2;
typedef CGAL::Polygon_2<K_dreses> Polygon_2;
typedef CGAL::Polygon_2<K_dreses>::Vertex_iterator vi_cgal;

typedef std::vector<int> Polygon_gp_id;



#endif /* POLYGONS_H */
