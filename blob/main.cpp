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
    int width = 600;
    int height = 600;
    int pt_nbr = 2000;
    int r = 160;

    float resolution = 0.8;
    int implitude = 140;

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
        for (int i = 1; i <= pt_nbr; i++){
            float angle = i*mov_angle;

            float x = r*cos(angle) + width/2;
            float y = r*sin(angle) + height/2;
            float delta = Remap(noise.GetNoise(x*resolution+(float)framecount*2, y*resolution+(float)framecount*2), -1, 1, -implitude, implitude);

            shape[i].position = sf::Vector2f(x+delta*cos(angle), y+delta*sin(angle));
        }
            // shape[pt_nbr].position = shape[1].position;
        // smothen the last quarter
        // int interpolation_start = floor(pt_nbr*11/12);
        // for (int i = interpolation_start; i <= pt_nbr; ++i){
        //     float angle = i*mov_angle;

        //     float amount = Remap(i,interpolation_start,pt_nbr,0,1);
        //     float inter_x = r*cos(angle) + Lerp(delta[i].x, delta[pt_nbr - i +1].x, amount);
        //     float inter_y = r*sin(angle) + Lerp(delta[i].y, delta[pt_nbr - i +1].y, amount);

        //     shape[i].position = sf::Vector2f(inter_x, inter_y);
        // }


        window.clear();

        //drawing part
        window.draw(shape);
        
        window.display();
        
        //std::cout << std::to_string(framecount) + " \n";
        framecount++;

    }

    return 0;
}
