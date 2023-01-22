#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include "include/FastNoiseLite.h"
#include "color/color.hpp"
#include <iostream>


// h [0, 360] s [0, 100] l [0, 100] a [0, 255]
// sf::Color HSL_RGB(int h, int s, int l, int A) {
//  float R,G,B;
//  s /= 100;
//  l /= 100;

//  float a_ = s*fminf(l, l-1);
//  // float k = ((4 + h/30) % 12);

//  R = l - a_*fmax(fmin(((h/30) % 12)-3, fmin(9-((h/30) % 12), 1)), -1);
//  G = l - a_*fmax(fmin(((8 + h/30) % 12)-3, fmin(9-((8 + h/30) % 12), 1)), -1);
//  B = l - a_*fmax(fmin(((4 + h/30) % 12)-3, fmin(9-((4 + h/30) % 12), 1)), -1);
//  std::cout << std::to_string(R) + " \n";
//  return sf::Color(R, G, B, A);
// }

int main()
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    FastNoiseLite warping;
    warping.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

	int framecount = 0;
    int width = 600;
    int height = 600;
    int cell_size = 4;
    float scale = 4;
    int implitude = 40; 

    int cols = floor(width/cell_size);
    int rows = floor(height/cell_size);

    //.... warping...
    // float warp[cols][rows];
    // sf::Texture warp_texture;
    // warp_texture.create(cols, rows);
    // sf::Sprite warp_sprite;
    // warp_sprite.setTexture(warp_texture);
    // warp_sprite.setPosition(cols,0);
    // sf::Uint8* warp_pixels = new sf::Uint8[cols * rows * 4];


    float cells[cols][rows]; 

    sf::Uint8* pixels = new sf::Uint8[cols * rows * 4];

    sf::Texture texture;
    texture.create(cols, rows);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(cell_size, cell_size));

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
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j) {
                float warp_x = warping.GetNoise((float)i*10,(float)j*10, (float)framecount);
                float warp_y = noise.GetNoise((float)i*10+200,(float)j*10+200, (float)framecount);
                cells[i][j] = noise.GetNoise(i*scale + 20*warp_x, j*scale + 20*warp_y);
            }
        }


        window.clear();

        //drawing part
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j){
                float valeur = Remap(cells[i][j],-1,1,0,360 );

                color::hsv<double> h( { valeur, 80, 50 } );
                color::rgb<double> r;
                r = h;

                pixels[(i + j*cols)*4 + 0] = floor(::color::get::red( r )*255);
                pixels[(i + j*cols)*4 + 1] = floor(::color::get::green( r )*255);
                pixels[(i + j*cols)*4 + 2] = floor(::color::get::blue( r )*255);
                pixels[(i + j*cols)*4 + 3] = 255;
            }
        }

        //warping
        // for (int i = implitude; i < cols-implitude; ++i) {
        //     for (int j = implitude; j < rows-implitude; ++j) {
        //         float offset = warp[i][j]*implitude;
        //         int off_x = i + offset;
        //         // int off_y = j + offset;
        //         pixels[(off_x + j*cols)*4 + 0] = pixels[(i + j*cols)*4 + 0];
        //         pixels[(off_x + j*cols)*4 + 1] = pixels[(i + j*cols)*4 + 1];
        //         pixels[(off_x + j*cols)*4 + 2] = pixels[(i + j*cols)*4 + 2];

        //     }
        // }



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