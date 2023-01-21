#include <SFML/Graphics.hpp>
#include <raymath.h>
#include <string>
#include <iostream>



int main()
{
    int framecount = 0;
    int width = 600;
    int height = 600;
    int part_nbr = 600;
    int speed = 4;

    Vector2 particle[part_nbr];
    srand (time(NULL));
    for(int i = 0; i < part_nbr; i++) {
        particle[i].x = rand() % width;
        particle[i].y = rand() % height;
    }

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
        for(int i = 0; i < part_nbr-1 ; i++) {
            //generate force vector
            Vector2 force;
            force.x = particle[i+1].x - particle[i].x;
            force.y = particle[i+1].y - particle[i].y;
            force = Vector2Scale(Vector2Normalize(force), speed);

            //upply vecroe
            particle[i] = Vector2Add(particle[i], force);
        }
        //last particle
            Vector2 force;
            force.x = particle[0].x - particle[part_nbr-1].x;
            force.y = particle[0].y - particle[part_nbr-1].y;
            force = Vector2Scale(Vector2Normalize(force), 4);
            particle[part_nbr-1] = Vector2Add(particle[part_nbr-1], force);

        //set last particle pos to mouse pos
        // sf::Vector2i mousepos = sf::Mouse::getPosition(window);
        // particle[part_nbr - 1].x = mousepos.x;
        // particle[part_nbr - 1].y = mousepos.y;


        window.clear();

        //drawing part
        for(int i = 0; i < part_nbr; i++) {
        sf::CircleShape circle;
        circle.setRadius(1);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(0);
        circle.setPosition(particle[i].x, particle[i].y);
        window.draw(circle);
        }

        window.display();

        // sf::Texture texture;
        // texture.create(window.getSize().x, window.getSize().y);
        // texture.update(window);
        // std::string filename = std::to_string(framecount);
        // texture.copyToImage().saveToFile("movio/" + filename + ".png");


        std::cout << std::to_string(framecount) + " \n";

        // if(framecount == 300) {
        //     window.close();
        // }
        framecount++;

    }

    return 0;
}