// Gmsh project created on Mon May 13 09:53:46 2013
Point(1) = {-1.1, 0.6, 0, 1.0};
Point(2) = {0.4, 0.6, 0, 1.0};
Point(3) = {0.4, -0.5, 0, 1.0};
Point(4) = {-1.1, -0.5, 0, 1.0};
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};
Line Loop(5) = {1, 2, 3, 4};
Plane Surface(6) = {5};
Extrude {0, 0, 1} {
  Surface{6};
}
