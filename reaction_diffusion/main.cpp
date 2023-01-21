#include <SFML/Graphics.hpp>
#include <raymath.h>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>



int main()
{
    srand (time(NULL));

    int framecount = 0 ;
    int width = 900;
    int height = 650;
    int cell_size = 2;

    int cols = floor(width/cell_size);
    int rows = floor(height/cell_size);

    float Da = 1;
    float Db = 0.5;
    float f = 0.054;
    float k = 0.062;
    float lapmap[] = {0.05, 0.2, 0.05, 0.2, -1, 0.2, 0.05, 0.2, 0.05};

    float A[cols][rows];
    float A_buffer[cols][rows];
    float B[cols][rows];
    float B_buffer[cols][rows];
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            // A[i][j] = (rand() / (RAND_MAX + 1.))/2 + 0.5;
            B[i][j] = (rand() / (RAND_MAX + 1.))/2 + 0.5;
            A[i][j] = 0;
        }
    }
    std::copy(&A[0][0], &A[0][0] + cols*rows, &A_buffer[0][0]);
    std::copy(&B[0][0], &B[0][0] + cols*rows, &B_buffer[0][0]);


    sf::Uint8* pixels = new sf::Uint8[cols * rows * 4];


    sf::Texture texture;
    texture.create(cols, rows);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(cell_size, cell_size));


    sf::RenderWindow window(sf::VideoMode(width, height), "reaction diffusion");
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
                float laplace_A = 0;
                float laplace_B = 0;

                // calculate laplace_A
                for (int x = -1; x <= 1; ++x) {
                    for (int y = -1; y <= 1; ++y){
                    int col = (i + x + cols) % cols;
                    int row = (j + y + rows) % rows;
                    laplace_A += A[col][row] * lapmap[(x+2) + (y+2) * 5];
                    }
                }

                // calculate laplace_B
                for (int x = -1; x <= 1; ++x) {
                    for (int y = -1; y <= 1; ++y){
                    int col = (i + x + cols) % cols;
                    int row = (j + y + rows) % rows;
                    laplace_B += B[col][row] * lapmap[(x+2) + (y+2) * 5];
                    }
                }

                A_buffer[i][j] = A[i][j] + Da*laplace_A - A[i][j]*B[i][j]*B[i][j] + f*(1 - A[i][j]);
                B_buffer[i][j] = B[i][j] + Db*laplace_B + A[i][j]*B[i][j]*B[i][j] - B[i][j]*(k + f);
            }
        }
        std::copy(&A_buffer[0][0], &A_buffer[0][0] + cols*rows, &A[0][0]);
        std::copy(&B_buffer[0][0], &B_buffer[0][0] + cols*rows, &B[0][0]);






        window.clear();

        // generate the pixels array
        for (int i = 0; i < cols; ++i) {
            for (int j = 0; j < rows; ++j){
                int valeur = 255 * Remap((B[i][j] - A[i][j]),-1,1,0,1 );
                pixels[(i + j*cols)*4 + 0] = pixels[(i + j*cols)*4 + 1] = pixels[(i + j*cols)*4 + 2] = floor(valeur);
                pixels[(i + j*cols)*4 + 3] = 255;
            }
        }

        //generate texture from the pixels array
        texture.update(pixels);

        // generate sprite
        sprite.setTexture(texture);
        window.draw(sprite);



        window.display();



        framecount++;

    }

    return 0;
}