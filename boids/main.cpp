#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>

struct boid {
    sf::Vector2f pos;
    sf::Vector2f vel;
};

int framecount = 0;
int const WIDTH = sf::VideoMode::getDesktopMode().width;
int const HEIGHT = sf::VideoMode::getDesktopMode().height;
float G = 1;

sf::Vector2f max_speed(6.f, 6.f);
int const boids_nbr = 800;
boid boids[boids_nbr];
std::vector<int> in_range;

int separation_range = 40;
int alignment_range = 60;
int cohesion_range = 50;
float turnfactor = 0.32;


float separation_force = 0.004;
float alignment_force = 0.01;
float cohesion_force = 0.03;

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

void insertionSort(std::vector<int> arr, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && Vector2Distance(boids[arr[j]].pos , boids[key].pos) <= Vector2Distance(boids[key].pos , boids[j].pos))
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    bool mouse_was_pressed = false;

    srand (time(NULL));

    for (int i = 0; i < boids_nbr; ++i){
        boids[i].pos = sf::Vector2f(rand() % WIDTH, rand() % HEIGHT);
        boids[i].vel = Vector2Normalize( sf::Vector2f(rand() % 2*WIDTH - WIDTH, rand() % 2*HEIGHT - HEIGHT) ) * 8.f;
    }

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setFillColor(sf::Color::White);
    triangle.setPoint(0, sf::Vector2f(0, -4));
    triangle.setPoint(1, sf::Vector2f(0, 4));
    triangle.setPoint(2, sf::Vector2f(10, 0));

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
        #pragma acc parallel loop
        for (int i = 0; i < boids_nbr; ++i){
            //detect  in_range boids
            // int min_range = std::min(alignment_range, std::min(cohesion_range, separation_range));
            // for (int j = 0; j < boids_nbr; ++j){
            //     if (j != i && Vector2Distance(boids[i].pos , boids[j].pos) <= min_range){
            //         in_range.push_back(j);
            //     }
            // }

            //sort in range boids by dist to boid[i]
            // insertionSort(in_range, in_range.size());
            // for (int index = 1; index < in_range.size(); index++){
            //     int new_pos = -1;
            //     for (int j = index - 1; j >= 0; j--){
            //         if(Vector2Distance(boids[in_range[i]].pos , boids[in_range[index]].pos) <= Vector2Distance(boids[in_range[i]].pos , boids[in_range[j]].pos)) {
            //             new_pos = j;
            //         }
            //     }

            //     //set it to its new position
            //     if(new_pos >= 0) {
            //         int dep =  in_range[index];
            //         in_range.insert(in_range.begin() + new_pos, dep);
            //         in_range.erase(in_range.begin()+ index+1);
            //     }
            // }

            // for (int index = 0; index < in_range.size(); index++) {
            //     std::cout << Vector2Distance(boids[i].pos , boids[index].pos) << " ";
            // }
            // std::cout << " \n";
            

            sf::Vector2f v1 = separation(i);
            sf::Vector2f v2 = alignment(i);
            sf::Vector2f v3 = cohesion(i);

            if(mouse_was_pressed) {
                sf::Vector2f force = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) - boids[i].pos;
                float amp = (G * 50)/Vector2Length(force) * Vector2Length(force);
                boids[i].vel += Vector2Normalize(force)*amp;
            }
            boids[i].vel += v1*separation_force + v2*alignment_force + v3*cohesion_force ;
            boids[i].vel = Vector2Clamp(boids[i].vel, -max_speed, max_speed);
            boids[i].pos += boids[i].vel;

            //screen edges: wrap arrond
            // {
            // if (boids[i].pos.x > WIDTH) {
            //     boids[i].pos.x = 0;
            // }
            // if (boids[i].pos.x < 0) {
            //     boids[i].pos.x = WIDTH;
            // }
            // if (boids[i].pos.y > HEIGHT) {
            //     boids[i].pos.y = 0;
            // }
            // if (boids[i].pos.y  < 0) {
            //     boids[i].pos.y = HEIGHT;
            // }
            // }

            // //screen edge: repultion force methode
            {
            if (boids[i].pos.x > WIDTH - WIDTH*0.2) {
                boids[i].vel.x -= turnfactor;
            }
            if (boids[i].pos.x < WIDTH*0.2) {
                boids[i].vel.x += turnfactor;
            }
            if (boids[i].pos.y > HEIGHT - HEIGHT*0.2) {
                boids[i].vel.y -= turnfactor;
            }
            if (boids[i].pos.y  < HEIGHT*0.2) {
                boids[i].vel.y += turnfactor;
            }
            }

        }
        mouse_was_pressed = false;

        window.clear();
        //drawing part
        for (int i = 0; i < boids_nbr; ++i){
            triangle.setPoint(2, sf::Vector2f(Remap(Vector2Length(boids[i].vel), 0, max_speed.x, 4, 10), 0));
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