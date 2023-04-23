#include <SFML/Graphics.hpp>
#include "../glm/ext.hpp"
#include <stdio.h>

#include "include/particle_system.h"

sf::Vector2f GLM2SF(glm::vec2 input)
{
    return sf::Vector2(input.x, input.y);
}
float random(int max)
{
    return rand() % max;
}

int framecount = 0;
int WIDTH = 600;
int HEIGHT = 400;

int part_nbr = 120;
int subSteps = 8;
std::vector<glm::vec2> positions;
std::vector<glm::vec2> accelerations;
glm::vec2 gravity(0, 2);
// int part_raduis = 8;

int main()
{
    //////// SETUP
    for (int i = 0; i < part_nbr; i++)
    {
        positions.push_back({random(WIDTH), random(HEIGHT)});
        accelerations.push_back(glm::vec2(0));
    }

    particle_system world(positions, accelerations,{WIDTH, HEIGHT}, gravity, subSteps);

    sf::CircleShape circle(0, 20);
    circle.setOutlineThickness(0);
    circle.setFillColor(sf::Color::White);

    sf::Font font;
    if(!font.loadFromFile("Hack-Regular.ttf"))
        printf("DAMIT :(");
    sf::Text text_timer("0", font, 20);
    text_timer.setPosition(0, 0);
    text_timer.setFillColor(sf::Color::Red);

    sf::Clock timer;
    int elapsed;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            //     world.update(0.6);
            
            //add praticles
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                world.add_particle({glm::linearRand(0, WIDTH), glm::linearRand(0, HEIGHT/4)}, {0, 40}, glm::linearRand(0, 12));
                // printf("particle added: %d \n", world.m_part_nbr);
            }
        }

        //////// UPDATE
        timer.restart();
        world.update(0.6);
        elapsed = timer.getElapsedTime().asMilliseconds();
        // printf("%d\n", elapsed);
        text_timer.setString(std::to_string(elapsed));

        window.clear();
        //////// DRAW
        for (int i = 0; i < world.m_part_nbr; i++)
        {
            float& raduis = world.m_raduis[i];
            circle.setRadius(raduis);
            circle.setPosition(world.m_pos[i].x - raduis, world.m_pos[i].y - raduis);
            window.draw(circle);
        }
        window.draw(text_timer);
        window.display();

        


        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}