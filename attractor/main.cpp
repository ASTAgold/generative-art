//Hopalong Attractor

#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>

float a=-2.722, b=2.574, c=1.284 ,d=3;
// float a = 1.5;
// float b = 1.8;
// float c = -1.6;
// float d = 0.2;

float sign(float x) {
	if(x > 0) {
		return 1;
	}else if(x < 0) {
		return -1;
	}else {
		return 0;
	}
}


float X_fct(float x, float y) {
	// return y-1-sqrt(abs(b*x-1-c))*sign(x-1);
    return d*sin(x*a)-sin(y*b);
}
float Y_fct(float x, float y) {
	// return a-x-1;
    return c*cos(x*a)+cos(y*b);
}

// h [0, 360] s [0, 100] l [0, 100] a [0, 255]
// sf::Color HSL_RGB(int H, int S, int L, int A) {
// 	float R,G,B;
// 	S /= 100;
// 	L /= 100; 

//     float d = S*(1 - abs(2*L-1));
//     float m = 255*(L - 0.5*d);

//     float x = d*(1 - abs((H/60) % 2 - 1));

//     switch(H) {
//         case 0 ... 59:
//             R = 255*d + m;
//             G = 255*x + m;
//             B = m;       
//         case 60 ... 119:
//             R = 255*x + m;
//             G = 255*d + m;
//             B = m;
//         case 120 ... 179:
//             R = m;
//             G = 255*d + m;
//             B = 255*x + m;
//         case 180 ... 239:
//             R = m;
//             G = 255*x + m;
//             B = 255*d + m;
//         case 240 ... 299:
//             R = 255*x + m;
//             G = m;
//             B = 255*d + m;
//         case 300 ... 360:
//             R = 255*d + m;
//             G = m;
//             B = 255*x + m;
//     }
//     std::cout << std::to_string(R) + " \n";
//     std::cout << std::to_string(G) + " \n";
//     std::cout << std::to_string(B) + " \n";

//     return sf::Color(R,G,B);
// }


int main()
{
    srand (time(NULL));

	int framecount = 0;
    int width = 800;
    int height = 600;
    int pts_nbr = 10000;
    int scale = (fmin(width,height)/2)/fmax(abs(c)+1,abs(d)+1);
    sf::Color background_clr = sf::Color(40,40,40);
    sf::Color draw_clr = sf::Color(251, 241, 199, 30);

    float points_x[pts_nbr];
    float points_y[pts_nbr];
    points_x[0] = 0.1;
    points_y[0] = 0.1;

    
    // that one unfortunat point i will be repeetidly drawing :)
	sf::CircleShape point;
	point.setRadius(0.4);
	point.setOutlineThickness(0);
	point.setFillColor(sf::Color::White);


    sf::RenderWindow window(sf::VideoMode(width, height), "attractor");
    window.setFramerateLimit(60);


    sf::RenderTexture texture;
    texture.create(width, height);
    texture.clear(background_clr);

    //trail eefect rect
    sf::RectangleShape trans_rect(sf::Vector2f(width, height));
    trans_rect.setFillColor(sf::Color(33, 31, 31,80));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
            	
        		// std::string filename = std::to_string(framecount);
        		// texture.getTexture().copyToImage().saveToFile("mmm/" + filename + ".png");
                
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    a = rand() / (RAND_MAX + 1.)*6 - 3;
                    b = rand() / (RAND_MAX + 1.)*6 - 3;
                    c = rand() / (RAND_MAX + 1.)*6 - 3;
                    d = rand() / (RAND_MAX + 1.)*6 - 3;
                    // std::cout << std::to_string(a) + " \n";
                    // std::cout << std::to_string(b) + " \n";
                    // std::cout << std::to_string(c) + " \n \n";
                    texture.clear(background_clr);
                    points_x[0] = 0.1;
                    points_y[0] = 0.1;
                    scale = (fmin(width,height)/2)/fmax(abs(c)+1,abs(d)+1);
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    std::string filename = std::to_string(framecount);
                    texture.getTexture().copyToImage().saveToFile("mmm/" + filename + ".png");
                }
            }
        }


        // update part
		for(int i = 0; i < pts_nbr-1; i++) {
        	points_x[i+1] = X_fct(points_x[i], points_y[i]);
        	points_y[i+1] = Y_fct(points_x[i], points_y[i]);
        	// std::cout << std::to_string(points_x[i]) + " \n";
        }

        // texture.clear();
        window.clear();

        //drawing part
        for (int i = 0; i < pts_nbr; ++i) {
        	float display_x = points_x[i]*scale + width/2;
        	float display_y = points_y[i]*scale + height/2;
        	point.setPosition(display_x, display_y);
        	point.setFillColor(draw_clr);
        	texture.draw(point);
        }
        // texture.draw(trans_rect);

		sf::Sprite sprite(texture.getTexture());
		// sprite.setScale(sf::Vector2f(2, 2));
   		window.draw(sprite);


        window.display();

        points_x[0] = points_x[pts_nbr-1];
        points_y[0] = points_y[pts_nbr-1];

        // std::cout << std::to_string(framecount) + " \n";

        if(framecount == 3000) {
            texture.getTexture().copyToImage().saveToFile("mmm/attract_1.png");    
            window.close();
        }

        framecount++;
        // std::cout << std::to_string(framecount) + " \n";


    }

    return 0;
}
