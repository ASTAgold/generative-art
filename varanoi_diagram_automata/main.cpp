#include <SFML/Graphics.hpp>
// #include "include/raymath.h"
#include <iostream>
#include <cmath>

int framecount = 0;
const int WIDTH = 600;
const int HEIGHT = 600;

const int seed_nbr = 4;
sf::Vector2i seed_pos[seed_nbr];
sf::Color seed_color[seed_nbr];

float Vector2Distance(sf::Vector2i v1, sf::Vector2i v2)
{
    float result = sqrtf((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
    // float result = abs((v1.x - v2.x)) + abs((v1.y - v2.y));

    return result;
}
float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
    float result = (value - inputStart)/(inputEnd - inputStart)*(outputEnd - outputStart) + outputStart;

    return result;
}

int main()
{
    srand (time(NULL));

    //generate the seed positions/cell colors; 
    // for (int i = 0; i < seed_nbr; ++i){
    //     seed_pos[i] = sf::Vector2i(rand() % WIDTH, rand() % HEIGHT);
    //     seed_color[i] = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    // }

    sf::Image gradiant;
    gradiant.loadFromFile("resources/gradient.jpg");

    sf::Image image;
    image.create(HEIGHT, WIDTH, sf::Color::Black);

    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
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
        for (int i = 0; i < seed_nbr; ++i){
            seed_pos[i] = {rand() % WIDTH, rand() % HEIGHT};
            // Remap(seed_pos[i].x,0,WIDTH,0,255)
            // seed_color[i] = sf::Color(Remap(seed_pos[i].x,0,WIDTH,0,255), Remap(seed_pos[i].y,0,WIDTH,0,255), Remap(seed_pos[i].x - seed_pos[i].y,-HEIGHT,WIDTH,0,255));
            // seed_color[i] = sf::Color(Remap(cos(seed_pos[i].x),-1,1,0,255), 200, Remap(cos(seed_pos[i].y),-1,1,0,255));
            seed_color[i] = gradiant.getPixel(Remap(seed_pos[i].x,0,WIDTH,0,gradiant.getSize().x), Remap(seed_pos[i].y,0,HEIGHT,0,gradiant.getSize().y));

        }

        for (int i = 0; i < WIDTH; ++i){
            for (int j = 0; j < HEIGHT; ++j){
                int seed_index = 0;
                float last_seed_dist = RAND_MAX;
                for (int k = 0; k < seed_nbr; ++k){
                    float seed_dist = Vector2Distance(seed_pos[k], sf::Vector2i(i, j));
                    if(seed_dist < last_seed_dist){
                        last_seed_dist = seed_dist;
                        seed_index = k;
                    }

                    image.setPixel(i, j, seed_color[seed_index]);
                }
            }
        }

        window.clear();
        //drawing part
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();

        std::cout << std::to_string(framecount) + " \n";
        framecount++;
    }

    return 0;
}