/* 
This program demonstrates a simple projectile motion simulation given an initial velocity, angle of projection 
and acceleration due to gravity.
*/

#include <iostream>
#include <cmath>
#define PI 3.14159265358979323846

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
	double operator^(const Coords_3D& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Coords_3D operator%(const Coords_3D& other) const {
		return Coords_3D(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	Coords_3D operator+(const Coords_3D& other) const {
		return Coords_3D(x + other.x, y + other.y, z + other.z);
	}

	// Operator: scalar multiplication (vector * scalar)
	Coords_3D operator*(double scalar) const {
		return Coords_3D(x * scalar, y * scalar, z * scalar);
	}

	// Operator: scalar multiplication (scalar * vector)
	friend Coords_3D operator*(double scalar, const Coords_3D& vec) {
		return vec * scalar;
	}
	Coords_3D operator-() const {
		return Coords_3D(-x, -y, -z);
	}

};
enum class PhysicalObjectType {
	Sphere,
	Cylinder,
	Cone,
	Cuboid,


};
std::string to_string(PhysicalObjectType type) {
	switch (type) {
	case PhysicalObjectType::Sphere:
		return "Sphere";
	case PhysicalObjectType::Cylinder:
		return "Cylinder";
	case PhysicalObjectType::Cone:
		return "Cone";
	case PhysicalObjectType::Cuboid:
		return "Cuboid";
	default:
		return "Unknown";
	}
}
std::ostream& operator<<(std::ostream& os, const PhysicalObjectType& type) {
	os << to_string(type);
	return os;
}
struct PhysicalObjectDefn
{
	PhysicalObjectType obj;
	double mass;  // Mass of the object
	Coords_3D position;  // Position of the centre of the object in 3D space

	// Characteristic definers of the object
	double radius;  // Radius for Sphere
	double height;  // Height for Cylinder and Cone
	double width;   // Width for Cuboid
	double length;  // Length for Cuboid
	double base_radius; // Base radius for Cone and Cylinder
	std::string name;  // Name of the object




	PhysicalObjectDefn(PhysicalObjectType type, double m, const Coords_3D& pos,
		double r = 0.0, double h = 0.0, double w = 0.0, double l = 0.0, double br = 0.0, const std::string& n = "")
		: obj(type), mass(m), position(pos), radius(r), height(h), width(w), length(l), base_radius(br), name(n) {
	}
	void print() const {
		std::cout << "Object: " << name << "\n"
			<< "Type: " << obj << "\n"
			<< "Mass: " << mass << "\n"
			<< "Position: ";
		position.print();
		std::cout << "Radius: " << radius << ", Height: " << height
			<< ", Width: " << width << ", Length: " << length
			<< ", Base Radius: " << base_radius << "\n";
	}

};
class ShapeRenderer {
public:
	static void render(const PhysicalObjectDefn& obj) {
		std::cout << "Rendering object: " << obj.name << "\n";
		std::cout << "  Type: " << obj.obj << "\n";
		std::cout << "  Position: ";
		obj.position.print();
		print_bounds(obj);
		std::cout << "\n";
	}

private:
	static void print_bounds(const PhysicalObjectDefn& obj) {
		double min_x, max_x, min_y, max_y, min_z, max_z;
		switch (obj.obj) {
		case PhysicalObjectType::Sphere: {
			double r = obj.radius;
			min_x = obj.position.x - r;
			max_x = obj.position.x + r;
			min_y = obj.position.y - r;
			max_y = obj.position.y + r;
			min_z = obj.position.z - r;
			max_z = obj.position.z + r;
			break;
		}

		case PhysicalObjectType::Cuboid: {
			min_x = obj.position.x - obj.width / 2.0;
			max_x = obj.position.x + obj.width / 2.0;
			min_y = obj.position.y - obj.length / 2.0;
			max_y = obj.position.y + obj.length / 2.0;
			min_z = obj.position.z - obj.height / 2.0;
			max_z = obj.position.z + obj.height / 2.0;
			break;
		}

		case PhysicalObjectType::Cylinder:
		case PhysicalObjectType::Cone: {
			min_x = obj.position.x - obj.base_radius;
			max_x = obj.position.x + obj.base_radius;
			min_y = obj.position.y - obj.base_radius;
			max_y = obj.position.y + obj.base_radius;
			min_z = obj.position.z;
			max_z = obj.position.z + obj.height;
			break;
		}
		default:
			min_x = min_y = min_z = 0.0;
			max_x = max_y = max_z = 0.0;
			std::cerr << "Unknown object type, cannot compute bounds.\n";
			return; // Exit if the object type is unknown
		}
		double box_volume = (max_x - min_x) * (max_y - min_y) * (max_z - min_z);
		std::cout << "  Bounding Coordinates:\n";
		std::cout << "    X: [" << min_x << ", " << max_x << "]\n";
		std::cout << "    Y: [" << min_y << ", " << max_y << "]\n";
		std::cout << "    Z: [" << min_z << ", " << max_z << "]\n";
		std::cout << "  Volume: " << compute_shape_volume(obj) << "\n";
		std::cout << "Volume of enclosing cuboid" << box_volume << "\n";
		std::cout<< "Wasteed space: " << (box_volume - compute_shape_volume(obj)) << "\n";
	}
	static double compute_shape_volume(const PhysicalObjectDefn& obj) {
		switch (obj.obj) {
		case PhysicalObjectType::Sphere:
			return (4.0 / 3.0) * PI * std::pow(obj.radius, 3);

		case PhysicalObjectType::Cuboid:
			return obj.width * obj.length * obj.height;

		case PhysicalObjectType::Cylinder:
			return PI * std::pow(obj.base_radius, 2) * obj.height;

		case PhysicalObjectType::Cone:
			return (1.0 / 3.0) * PI * std::pow(obj.base_radius, 2) * obj.height;

		default:
			return 0.0;
		}
	}

};

int main() {
	std::cout << "=== Vector Operations Test ===\n\n";

	Coords_3D position(4.0, 5.0, 6.0);
	Coords_3D position2(1.0, 2.0, 3.0);

	std::cout << "Position 1: ";
	position.print();

	std::cout << "Position 2: ";
	position2.print();

	std::cout << "\nDistance between points: " << position.distance(position2) << "\n";
	std::cout << "Magnitude of Position 1: " << position.magnitude() << "\n";
	std::cout << "Dot product (position · position2): " << (position ^ position2) << "\n";

	Coords_3D cross_product = position % position2;
	std::cout << "Cross product (position × position2): ";
	cross_product.print();

	Coords_3D sum = position + position2;
	std::cout << "Sum (position + position2): ";
	sum.print();

	Coords_3D scalar_mult = position2 * 2.0;
	std::cout << "Scalar multiplication (position2 * 2): ";
	scalar_mult.print();

	std::cout << "\n=== Physical Object Definition Test ===\n\n";

	PhysicalObjectDefn sphere(
		PhysicalObjectType::Sphere,
		5.0,
		position,
		1.0,   // radius
		0.0,   // height
		0.0,   // width
		0.0,   // length
		0.0,   // base_radius
		"MySphere"
	);
	PhysicalObjectDefn cylinder(
		PhysicalObjectType::Cylinder,
		10.0,
		position2,
		0.0,   // radius
		5.0,   // height
		0.0,   // width
		0.0,   // length
		2.0,   // base_radius
		"MyCylinder"
	);
	PhysicalObjectDefn cone(
		PhysicalObjectType::Cone,
		8.0,
		position,
		0.0,   // radius
		6.0,   // height
		0.0,   // width
		0.0,   // length
		3.0,   // base_radius
		"MyCone"
	);
	PhysicalObjectDefn cuboid(
		PhysicalObjectType::Cuboid,
		12.0,
		position2,
		0.0,   // radius
		3.0,   // height
		4.0,   // width
		5.0,   // length
		0.0,   // base_radius
		"MyCuboid"
	);

	sphere.print();
	ShapeRenderer::render(sphere);
	cylinder.print();
	ShapeRenderer::render(cylinder);
	cone.print();
	ShapeRenderer::render(cone);
	cuboid.print();
	ShapeRenderer::render(cuboid);

	return 0;
}



