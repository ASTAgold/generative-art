#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>
#include <complex>

int framecount = 0;
//width > height
int width = 400;
int height = 400;

double initial_scale = 1.5/height;

// float scale = 1;

// int texture_width = width*scale;
// int texture_height = height*scale;

sf::Uint8* pixels = new sf::Uint8[width * height * 4];

int iteration = 100;
double Z_x = 0;
double Z_y = 0;
double startx = -width*initial_scale;
double endx = width*initial_scale;
double starty = -1.5;
double endy = 1.5;

double new_startx, new_endx, new_starty, new_endy;

int zoom_width = width / 2;
int zoom_height = height / 2;

int is_in_set(std::complex<double> c) {
    std::complex<double> z (Z_x, Z_y);

    for (int i = 0; i < iteration; ++i){
        z = pow(z, 2) + c;
        if(std::abs(z) >= 2) {
            return i;
        }
    }

    // std::cout << std::to_string(n) + " \n";
    return iteration;
}

void campute_set() {
    for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            double x = Remap(i,0,width,startx, endx);
            double y = Remap(j,0,height,starty, endy);

            std::complex<double> nbr (x, y);

            int iter_nbr = is_in_set(nbr);
            // std::cout << std::to_string(iter_nbr) + " \n";


            int index = (i + j*width)*4;
            if(iter_nbr == iteration) {
                pixels[index] = pixels[index+1] =pixels[index+2] = 0;
            }else {
                    //HSL MAPPING

                float hue = Remap(iter_nbr,0,iteration,0,360);
                color::hsv<double> h( { hue, 80, 100 } );
                color::rgb<double> r;
                r = h;

                pixels[index+0] = floor(::color::get::red( r )*255);
                pixels[index+1] = floor(::color::get::green( r )*255);
                pixels[index+2] = floor(::color::get::blue( r )*255);

                    // SQRT COLORING

                // float bright = Remap(iter_nbr,0,iteration,0,1);
                // pixels[index] = pixels[index+1] =pixels[index+2] = Remap(sqrt(bright),0,1,0,255);

                    //  smt related to hue?
                // float hue = (iter_nbr / 1 ) - floor(iter_nbr / 1);
                // color::hsv<double> h( { hue, 80, 50 } );
                // color::rgb<double> r;
                // r = h;

                // pixels[index+0] = floor(::color::get::red( r )*255);
                // pixels[index+1] = floor(::color::get::green( r )*255);
                // pixels[index+2] = floor(::color::get::blue( r )*255);

            }
            pixels[index+3] = 255;
        }
    }

}

int main()
{
        std::cout << std::to_string(initial_scale) + " \n";
    std::cout << std::to_string(endx) + " \n";

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

        //mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            // std::cout << std::to_string(mouse_pos.x) + " ";
            // std::cout << std::to_string(mouse_pos.y) + " \n";

            new_startx = Remap(mouse_pos.x - zoom_width/2,0,width,startx, endx);
            // std::cout << std::to_string(new_startx) + " ";
            new_endx = Remap(mouse_pos.x + zoom_width/2,0,width,startx, endx);
            // std::cout << std::to_string(new_endx) + " \n";
            new_starty = Remap(mouse_pos.y - zoom_height/2,0,height,starty, endy);
            // std::cout << std::to_string(new_starty) + " ";
            new_endy = Remap(mouse_pos.y + zoom_height/2,0,height,starty, endy);
            // std::cout << std::to_string(endx) + " \n";

            startx = new_startx;
            endx = new_endx;
            starty  =  new_starty;
            endy = new_endy;

            campute_set();
        }
        //keyboard iteration count
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            iteration += 20;
            campute_set();
        }

        // update part


        window.clear();

        //drawing part

        //generate texture from the pixels array
        texture.update(pixels);
        
        // generate sprite
        sprite.setTexture(texture);
        window.draw(sprite);

        window.display();




        // std::cout << std::to_string(framecount) + " \n";


        framecount++;

    }

    return 0;
}