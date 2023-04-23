#include <SFML/Graphics.hpp>
#include "../glm/ext.hpp"
#include <stdio.h>
#include ""

#define PI 3.14159265359

sf::Vector2f GLM2SF(glm::vec2 input)
{
    return sf::Vector2(input.x, input.y);
}

int framecount = 0;
int WIDTH = 600;
int HEIGHT = 600;

float f = 0;
float h_sens = 0.2;
float implitude = 80;

int main()
{
    //////// SETUP
    sf::Clock clock;

    sf::Image temp;
    temp.create(WIDTH, HEIGHT);

    sf::Image image;
    image.create(WIDTH, HEIGHT);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            f += 0.002;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            f -= 0.002;
        }

        //////// UPDATE

        image = temp;
        float t = clock.getElapsedTime().asSeconds();
        for (int i = 0; i < WIDTH; i++)
        {
            float x = t + i*hSens;
            float y = HEIGHT/2 + cos(2*PI*f * x)*implitude;
            image.setPixel(i, y, {255, 255, 255});
        }
        

        window.clear();
        //////// DRAW

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);

        window.display();

        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}