#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include "color/color.hpp"
#include <iostream>
#include <complex>
#include <math.h>

//variables

int framecount = 0;
int width = 600;
int height = 600;

double startx = -width/2;
double endx = width/2;
double starty = -width/2;
double endy = width/2;

int limit = 1;

sf::Uint8* pixels = new sf::Uint8[width * height * 4];

void campute_set() {
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            float x = Remap(i,0,width,startx, endx);
            float y = Remap(j,0,height,starty, endy);

            float difference = (fabs(x) + fabs(y)) - fabs(x + y);

            int index = (i + j*width)*4;

            int color = Remap(difference,0,limit,0,255);
            pixels[index] = pixels[index+1] =pixels[index+2] = floor(color);
            pixels[index+3] = 255;
        }
    }

}

int main()
{
    campute_set();

    sf::Texture texture;
    texture.create(width, height);

    sf::Sprite sprite;
    sprite.setTexture(texture);

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
        limit += 2;
        campute_set();


            window.clear();

        //drawing part

        //generate texture from the pixels array
        texture.update(pixels);
        
        // generate sprite
        sprite.setTexture(texture);
        window.draw(sprite);


            window.display();

        // std::string filename = std::to_string(framecount);
        // texture.copyToImage().saveToFile("movio/" + filename + ".png");
        
        framecount++;

    }

    return 0;
}