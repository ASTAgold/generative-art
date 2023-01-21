#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include "include/FastNoiseLite.h"
#include <iostream>


int main()
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

	int framecount = 0;
    int width = 600;
    int height = 600;
    float cell_size = 2;
    float step = 0.3;
    float scale = 3;

    std::string gradiantplace = "resources/gradient1.png";

    sf::Image gradiant;
    gradiant.loadFromFile(gradiantplace);

    int cols = floor(width/cell_size);
    int rows = floor(height/cell_size);

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
        // step += 0.002;
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j) {
                cells[i][j] = noise.GetNoise((float)i*scale, (float)j*scale, (float)framecount);
            }
        }
        // scale += 0.01;

        window.clear();

        //drawing part
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j){
                int valeur = gradiant.getSize().x * Remap(cells[i][j],-1,1,0,1 );


                sf::Color color = gradiant.getPixel(valeur, 0);

                pixels[(i + j*cols)*4 + 0] = color.r;
                pixels[(i + j*cols)*4 + 1] = color.g;
                pixels[(i + j*cols)*4 + 2] = color.b;
                pixels[(i + j*cols)*4 + 3] = 255;
            }
        }

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