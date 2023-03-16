#include <SFML/Graphics.hpp>
#include "../glm/ext.hpp"
#include <stdio.h>

sf::Vector2f GLM2SF(glm::vec2 input)
{
    return sf::Vector2(input.x, input.y);
}

int framecount = 0;
int WIDTH = 600;
int HEIGHT = 600;

int main()
{
    //////// SETUP

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

        //////// UPDATE

        window.clear();
        //////// DRAW

        window.display();

        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}