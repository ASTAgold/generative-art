#include <SFML/Graphics.hpp>
#include "raymath.h"
#include <iostream>
#include <math.h>
#include <array>
#include <vector>

struct _circle {
	float r;
	sf::Vector2f O;
};

_circle calcCircumCirc(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2) {
	float A = v1.x - v0.x;
	float B = v1.y - v0.y;
	float C = v2.x - v0.x;
	float D = v2.y - v0.y;

	float E = A * (v0.x + v1.x) + B * (v0.y + v1.y);
	float F = C * (v0.x + v2.x) + D * (v0.y + v2.y);

	float G = 2.0 * (A * (v2.y - v1.y) - B * (v2.x - v1.x));

	float dx, dy;

	sf::Vector2f center;
	// Collinear points, get extremes and use midpoint as center
	if (std::round(abs(G)) == 0) {
		float minx = std::min(std::min(v0.x, v1.x), v2.x);
		float miny = std::min(std::min(v0.y, v1.y), v2.y);
		float maxx = std::max(std::max(v0.x, v1.x), v2.x);
		float maxy = std::max(std::max(v0.y, v1.y), v2.y);
		
	    center =  sf::Vector2f((minx + maxx) / 2, (miny + maxy) / 2);
	
		dx = center.x - minx;
		dy = center.y - miny;
	} else {
	    float cx = (D * E - B * F) / G;
	    float cy = (A * F - C * E) / G;
	
	    center = sf::Vector2f(cx, cy);
	
		dx = center.x - v0.x;
		dy = center.y - v0.y;
	}
	float radius = sqrt(dx * dx + dy * dy);

	_circle circumCircle;
	circumCircle.r = radius;
	circumCircle.O = center;

	return circumCircle;
};

class segment {
public:
	segment(sf::Vector2f arg_v0, sf::Vector2f arg_v1) {
		v0 = arg_v0;
		v1 = arg_v1;
	};
	bool equals(segment edge){return (v0 == edge.v0 && v1 == edge.v1) || (v0 == edge.v1 && v1 == edge.v0);};

	sf::Vector2f v0;
	sf::Vector2f v1;
};
class _triangle {
public:
	_triangle(sf::Vector2f arg_v0, sf::Vector2f arg_v1, sf::Vector2f arg_v2) {
		v0 = arg_v0;
		v1 = arg_v1;
		v2 = arg_v2;

		circumCirc = calcCircumCirc(v0,v1,v2);
	};
	bool inCircumcircle(sf::Vector2f v) {
		float dx = circumCirc.O.x - v.x;
		float dy = circumCirc.O.y - v.y;
		return sqrt(dx * dx + dy * dy) < circumCirc.r;// <= or < ??
	};

	sf::Vector2f v0;
	sf::Vector2f v1;
	sf::Vector2f v2;
	_circle circumCirc;
};

template<std::size_t pts_nbr>
_triangle calcSuperTriangle(std::array<sf::Vector2f,pts_nbr> vertices) {
	float minx, miny, maxx, maxy;
	minx = miny = INFINITY;
	maxx = maxy = -INFINITY;
	for(const sf::Vector2f &vertex : vertices) {
		minx = std::min(minx, vertex.x);
		miny = std::min(minx, vertex.y);
		maxx = std::max(maxx, vertex.x);
		maxy = std::max(maxx, vertex.y);
	}

	float dx = (maxx - minx) * 10;
	float dy = (maxy - miny) * 10;

	sf::Vector2f v0 (minx - dx, miny - dy * 3);
	sf::Vector2f v1 (minx - dx, maxy + dy);
	sf::Vector2f v2 (maxx + dx * 3, maxy + dy);

	_triangle superTriangle (v0, v1, v2);
	return superTriangle;
};