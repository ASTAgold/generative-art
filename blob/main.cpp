#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <cmath>
// #include "color/color.hpp"
#include "include/FastNoiseLite.h"
#include <iostream>


int main()
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	int framecount = 0;
    int width = 800;
    int height = 800;
    int pt_nbr = 4000;
    int r = 250;
    float step = 0.2;
    int implitude = 150;

    float mov_angle = (2*PI)/pt_nbr;

    sf::VertexArray shape(sf::TriangleFan, pt_nbr+1);
    shape[0].position = sf::Vector2f(width/2, height/2);

    sf::RenderWindow window(sf::VideoMode(width, height), "dop :)");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update part
        for (int i = 1; i <= pt_nbr-1; i++){
            float angle = i*mov_angle;
            float x = r*cos(angle) + implitude*noise.GetNoise(i*step, (float)1, (float)framecount);
            float y = r*sin(angle) + implitude*noise.GetNoise(i*step, (float)1000, (float)framecount);
            shape[i].position = sf::Vector2f(x+width/2, y+height/2);
        }
            shape[pt_nbr].position = shape[1].position;

        window.clear();

        //drawing part
        window.draw(shape);
        
        window.display();
        
        //std::cout << std::to_string(framecount) + " \n";


        framecount++;

    }

    return 0;
}
