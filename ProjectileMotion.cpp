/* 
This program demonstrates a simple projectile motion simulation given an initial velocity, angle of projection 
and acceleration due to gravity.
*/

#include <iostream>
#include <cmath>

struct Coords_3D  // Structure to hold 3D Coordinates
{
	double x;
	double y;
	double z;
};

struct Coords_4D // Strcuture to hold 4D Coordinates
{
	double t; // time
	Coords_3D coords; // 3D coordinates
};


