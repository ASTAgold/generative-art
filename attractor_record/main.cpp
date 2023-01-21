//Hopalong Attractor

#include <SFML/Graphics.hpp>
#include <raymath.h>
#include <iostream>

#define a 0.831755
#define b 13.247130
#define c 11.070888

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
	return y-1-sqrt(abs(b*x-1-c))*sign(x-1);
}
float Y_fct(float x, float y) {
	return a-x-1;
}



int main()
{
	int framecount = 0;
    int width = 1000;
    int height = 650;
    int pts_nbr = 10000;
    int scale = 1.5;
    sf::Color background_clr = sf::Color(33, 31, 31);
    sf::Color draw_clr = sf::Color(171, 171, 171, 100);

    float points_x[pts_nbr];
    float points_y[pts_nbr];
    points_x[0] = 1;
    points_y[0] = 1;

    
    // that one unfortunat point i will be repeetidly drawing :)
	sf::CircleShape point;
	point.setRadius(0.4);
	point.setOutlineThickness(0);
	point.setFillColor(sf::Color::White);


    sf::RenderWindow window(sf::VideoMode(width, height), "dop :)");
    window.setFramerateLimit(60);


    sf::RenderTexture texture;
    texture.create(width, height);
    texture.clear(background_clr);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
            	
        		// std::string filename = std::to_string(framecount);
        		texture.getTexture().copyToImage().saveToFile("movio/img3.png");
                
                window.close();
            }
        }


        // update part
		for(int i = 0; i < pts_nbr-1; i++) {
        	points_x[i+1] = X_fct(points_x[i], points_y[i]);
        	points_y[i+1] = Y_fct(points_x[i], points_y[i]);
        	// std::cout << std::to_string(points_x[i]) + " \n";
        }

        // texture.clear();
        // window.clear();

        //drawing part
        for (int i = 0; i < pts_nbr; ++i) {
        	float display_x = points_x[i]*scale + width/2;
        	float display_y = points_y[i]*scale + height/2;
        	point.setPosition(display_x, display_y);
            point.setFillColor(draw_clr);
        	texture.draw(point);
        }

		// sf::Sprite sprite(texture.getTexture());
		// sprite.setScale(sf::Vector2f(2, 2));
   		// window.draw(sprite);


        // window.display();

        points_x[0] = points_x[pts_nbr-1];
        points_y[0] = points_y[pts_nbr-1];

        // std::cout << std::to_string(framecount) + " \n";


        framecount++;
        if((framecount % 100) == 0) {
            std::cout << "damn \n";
        }
        if(framecount == 1200) {
            texture.getTexture().copyToImage().saveToFile("movio/img10.png");

            
            window.close();

        }

    }

    return 0;
}
