#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>

struct boid {
    sf::Vector2f pos;
    sf::Vector2f vel;
};

int framecount = 0;
int const WIDTH = 600;
int const HEIGHT = 450;

sf::Vector2f max_speed(6.f, 6.f);
int const boids_nbr = 300;
boid boids[boids_nbr];

int separation_range = 20;
int alignment_range = 60;
int cohesion_range = 50;
float turnfactor = 0.5;

float separation_force = 0.045;
float cohesion_force = 0.03;
float alignment_force = 0.02;

sf::Vector2f separation(int index) {
    sf::Vector2f separation_vec(0, 0);
    for (int i = 0; i < boids_nbr; ++i){
        if (i != index && Vector2Distance(boids[index].pos , boids[i].pos) <= separation_range){
            separation_vec += (boids[index].pos - boids[i].pos);
        }
    }
    return separation_vec;
}

sf::Vector2f alignment(int index) {
    sf::Vector2f alignment_vec(0,0);
    int neighbors = 0;
    for (int i = 0; i < boids_nbr; ++i){
        if (i != index && Vector2Distance(boids[index].pos , boids[i].pos) <= alignment_range){
            alignment_vec += boids[i].vel;
            neighbors++;
        }
    }
    return Vector2Normalize(alignment_vec/(float)neighbors)*max_speed.x - boids[index].vel;
}

sf::Vector2f cohesion(int index) {
    sf::Vector2f cohesion_vec(0,0);
    int neighbors = 0;
    for (int i = 0; i < boids_nbr; ++i){
        if (i != index && Vector2Distance(boids[index].pos , boids[i].pos) <= cohesion_range){
            cohesion_vec += boids[i].pos;
            neighbors++;
        }
    }
    return cohesion_vec/(float)neighbors - boids[index].pos;

}

int main()
{
    bool mouse_was_pressed = false;

    srand (time(NULL));

    for (int i = 0; i < boids_nbr; ++i){
        boids[i].pos = sf::Vector2f(rand() % WIDTH, rand() % HEIGHT);
        boids[i].vel = Vector2Normalize( sf::Vector2f(rand() % 2*WIDTH - WIDTH, rand() % 2*HEIGHT - HEIGHT) ) * 8.f;
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            mouse_was_pressed = true;
        }

        // update part
        for (int i = 0; i < boids_nbr; ++i){
            sf::Vector2f v1 = separation(i);
            sf::Vector2f v2 = alignment(i);
            sf::Vector2f v3 = cohesion(i);

            if(mouse_was_pressed) {
                sf::Vector2f force = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) - boids[i].pos;
                boids[i].vel += Vector2Normalize(force)*5.f;
            }
            boids[i].vel += v1*separation_force + v2*alignment_force + v3*cohesion_force ;
            boids[i].vel = Vector2Clamp(boids[i].vel, -max_speed, max_speed);
            boids[i].pos += boids[i].vel;

            //screen edges: wrap arrond
            {
            if (boids[i].pos.x > WIDTH) {
                boids[i].pos.x = 0;
            }
            if (boids[i].pos.x < 0) {
                boids[i].pos.x = WIDTH;
            }
            if (boids[i].pos.y > HEIGHT) {
                boids[i].pos.y = 0;
            }
            if (boids[i].pos.y  < 0) {
                boids[i].pos.y = HEIGHT;
            }
            }

            //screen edge: repultion force methode
            // {
            // if (boids[i].pos.x > WIDTH-200) {
            //     boids[i].vel.x -= turnfactor;
            // }
            // if (boids[i].pos.x < 200) {
            //     boids[i].vel.x += turnfactor;
            // }
            // if (boids[i].pos.y > HEIGHT-150) {
            //     boids[i].vel.y -= turnfactor;
            // }
            // if (boids[i].pos.y  < 150) {
            //     boids[i].vel.y += turnfactor;
            // }
            // }

        }
        mouse_was_pressed = false;

        window.clear();
        //drawing part
        for (int i = 0; i < boids_nbr; ++i){
            sf::ConvexShape triangle;
            triangle.setPointCount(3);
            triangle.setFillColor(sf::Color::White);
            triangle.setPoint(0, sf::Vector2f(0, -3));
            triangle.setPoint(1, sf::Vector2f(0, 3));
            triangle.setPoint(2, sf::Vector2f(10, 0));
            triangle.setRotation(RAD2DEG*Vector2Angle(sf::Vector2f(1, 0), boids[i].vel));
            triangle.setPosition(boids[i].pos);
            window.draw(triangle);
        }
        window.display();


            // recording stuff

        // sf::Texture texture;
        // texture.create(WIDTH, HEIGHT);
        // texture.update(window);
        // std::string filename = std::to_string(framecount);
        // texture.copyToImage().saveToFile("movio/" + filename + ".png");

        // if(framecount == 375) {
        //     window.close();
        // }

        // std::cout << std::to_string(framecount) + " \n";
        framecount++;
    }

    return 0;
}