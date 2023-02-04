#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>

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

class _triangle {
public:
    _triangle(sf::Vector2f arg_v0, sf::Vector2f arg_v1, sf::Vector2f arg_v2) {
        v0 = arg_v0;
        v1 = arg_v1;
        v2 = arg_v2;

        circumCirc = calcCircumCirc(v0,v1,v2);
    };

    sf::Vector2f v0;
    sf::Vector2f v1;
    sf::Vector2f v2;
    _circle circumCirc;
};

int main()
{
    srand (time(NULL));

	int framecount = 0;
    int width = 600;
    int height = 600;

    sf::Vector2f points[3];
    for (int i = 0; i < 3; ++i){
        points[i] = sf::Vector2f(rand() % width, rand() % height);
    }

    _triangle triangle(points[0], points[1], points[2]);

    sf::RenderWindow window(sf::VideoMode(width, height), "dop :)");
    window.setFramerateLimit(2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update part

        for (int i = 0; i < 3; ++i){
            points[i] = sf::Vector2f(rand() % width, rand() % height);
        }
        triangle = _triangle(points[0], points[1], points[2]);


        window.clear();
        //drawing part
        sf::CircleShape circle;
        circle.setRadius(triangle.circumCirc.r);
        circle.setFillColor(sf::Color::Blue);
        circle.setOutlineThickness(0);
        circle.setPosition(triangle.circumCirc.O.x - triangle.circumCirc.r, triangle.circumCirc.O.y - triangle.circumCirc.r);
        window.draw(circle);

        sf::ConvexShape convex;

        convex.setPointCount(3);
        convex.setPoint(0, triangle.v0);
        convex.setPoint(1, triangle.v1);
        convex.setPoint(2, triangle.v2);

        window.draw(convex);

        window.display();

        // std::cout << std::to_string(framecount) + " \n";
        framecount++;
    }

    return 0;
}