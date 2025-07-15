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
	Coords_3D(double x_val = 0.0, double y_val = 0.0, double z_val = 0.0) 
		: x(x_val), y(y_val), z(z_val) {
	}
	double distance(const Coords_3D& other) const {
		return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
	}
	void print() const {
		std::cout << "Coordinates: (" << x << ", " << y << ", " << z << ")\n";
	}
	double magnitude() const {
		return std::sqrt(x * x + y * y + z * z);
	}
	double dot(const Coords_3D& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Coords_3D cross(const Coords_3D& other) const {
		return Coords_3D(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	Coords_3D operator+(const Coords_3D& other) const {
		return Coords_3D(x + other.x, y + other.y, z + other.z);
	}
	Coords_3D scale(double scale) const {
		return Coords_3D(x * scale, y * scale, z * scale);
	}
};



