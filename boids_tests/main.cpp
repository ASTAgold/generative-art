#include <SFML/Graphics.hpp>
#include <raymath.h>
#include <iostream>


int main()
{
	int framecount = 0;
    int width = 600;
    int height = 600;



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



        window.clear();

        //drawing part



        window.display();




        std::cout << std::to_string(framecount) + " \n";


        framecount++;

    }

    return 0;
}