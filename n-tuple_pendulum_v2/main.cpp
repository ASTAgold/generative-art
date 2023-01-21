#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>
#include <algorithm>

int framecount = 0;
const int width = 600;
const int height = 600;
const int pendulum_nbr = 4;
const float DT = 1;
const int substeps_nbr = 5;
const sf::Vector2f gravity (0, 2);

float pts_mass[pendulum_nbr] = {0,0.4,0.2,0.2};
float pts_lenght[pendulum_nbr] = {0,60,60,60};
float pts_initial_angles[pendulum_nbr] = {0,PI/2,PI/2,PI/2};

class pendulum {
public:
    float masses[pendulum_nbr];
    float lenghts[pendulum_nbr];
    sf::Vector2f pos[pendulum_nbr];
    sf::Vector2f prev_pos[pendulum_nbr];
    sf::Vector2f vel[pendulum_nbr];

    pendulum(float mass[],float lenght[]) {
        std::copy (mass, mass+pendulum_nbr, masses);
        std::copy (lenght, lenght+pendulum_nbr, lenghts);
        float x_i = 0;
        float y_i = 0;
        for (int i = 1; i < pendulum_nbr; ++i){
            x_i += lenghts[i]*sin(pts_initial_angles[i]);
            y_i += lenghts[i]*cos(pts_initial_angles[i]);

            pos[i] = sf::Vector2f(x_i,y_i);
            vel[i] = sf::Vector2f(0,0);
        }
    }
    void campute(float dt) {
        for (int i = 0; i < pendulum_nbr; ++i){
            vel[i] += gravity*dt;
            prev_pos[i] = pos[i];
            pos[i] += vel[i]*dt;
        }
        for (int i = 1; i < pendulum_nbr; ++i){
            //lenghts
            float dLenght = lenghts[i-1] - Vector2Distance(pos[i-1], pos[i]);
            float L1 = dLenght*(masses[i-1]/(masses[i-1]+masses[i]));
            float L2 = dLenght*(masses[i]/(masses[i-1]+masses[i]));

            sf::Vector2f v1 = sf::Vector2f(L1 * sin(Vector2Angle(sf::Vector2f(0,1), sf::Vector2f(pos[i]- pos[i-1]))), L1 * cos(Vector2Angle(sf::Vector2f(0,1), sf::Vector2f(pos[i]- pos[i-1]))));
            sf::Vector2f v2 = -sf::Vector2f(L2 * sin(Vector2Angle(sf::Vector2f(0,1), sf::Vector2f(pos[i]- pos[i-1]))), L2 * cos(Vector2Angle(sf::Vector2f(0,1), sf::Vector2f(pos[i]- pos[i-1]))));

            pos[i-1] += v1;
            pos[i] += v2;
        }
        for (int i = 0; i < pendulum_nbr; ++i){
            vel[i] = (pos[i] - prev_pos[i])*dt;
        }
    }
};

int main()
{
    pendulum pend(pts_mass, pts_lenght);

    sf::RenderWindow window(sf::VideoMode(width, height), "dop :)");
    window.setFramerateLimit(1);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // update part
        pend.campute(DT);

        window.clear();
        //drawing part
        for (int i = 0; i < pendulum_nbr; ++i){
            sf::CircleShape circle;
            circle.setRadius(10);
            circle.setOutlineThickness(0);
            circle.setPosition(width/2 + pend.pos[i].x,height/2 + pend.pos[i].y);
            window.draw(circle);
        }

        window.display();

        // std::cout << std::to_string(framecount) + " \n";
        framecount++;

    }

    return 0;
}