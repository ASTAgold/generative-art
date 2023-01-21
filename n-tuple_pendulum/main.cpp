#include <SFML/Graphics.hpp>
#include "include/raymath.h"
#include <iostream>
#include <valarray>

int  framecount = 0;
const int WIDTH = 600;
const int HEIGHT = 600;
const int PEND_NBR = 5;
const int LENGHT = 20;

void lusolve(int n, std::valarray<std::valarray<float> > a, std::valarray<float> b, std::valarray<float> x){
  int i,j;
  float s;

  for(i = 0; i < n; i++) {
        s = 0;
        for(j = 0; j < i; j++) {
            s = s + a[ i][ j] * x[ j];
        }
        x[ i] = ( b[ i] - s) / a[ i][ i];
   }
}

class pendulum {
public:
    static const int n = 5;
    float g = 9.8;
    float dt = 1/60;
    std::valarray<float> thetas  = std::valarray<float>(PI/6, n);
    std::valarray<float> thetaDots = std::valarray<float>(3, n);

    auto A(std::valarray<float> arg_thetas) {
        std::valarray<std::valarray<float>> M (std::valarray<float>(n),n);        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++){
                M[i][j] = (n - std::max(i, j)) * cos(arg_thetas[i] - arg_thetas[j]);
            }
        }
        return M;
    }
    auto b(std::valarray<float> arg_thetas, std::valarray<float> arg_thetaDots) {
        std::valarray<float> v (n);
        for (int i = 0; i < n; i++){
            float b_i = 0;
            for (int j = 0; j < n; j++){
                b_i -= (n - std::max(i, j)) * sin(arg_thetas[i] - arg_thetas[j]) * arg_thetaDots[j] *2*2;
            }
            b_i -= g * (n - i) * sin(arg_thetas[i]);
            v[i] = b_i;
        }
        return v;
    }
    auto f(std::valarray<float> arg_thetas, std::valarray<float> arg_thetaDots) {
        std::valarray<float> x (n);
        lusolve(n, A(arg_thetas), b(arg_thetas, arg_thetaDots), x);
        std::valarray<std::valarray<float>> result = {arg_thetaDots,x};
        return result;
    }
    
    auto RK4(float dt, std::valarray<float> arg_thetas, std::valarray<float> arg_thetaDots) {
        auto k1 = f(arg_thetas, arg_thetaDots);
        auto k2 = f(arg_thetas + k1[0]*(dt/2), arg_thetaDots + k1[1]*(dt/2));
        auto k3 = f(arg_thetas + k2[0]*(dt/2), arg_thetaDots + k2[1]*(dt/2));
        auto k4 = f(arg_thetas +k3[0]*dt, arg_thetaDots + k3[1]*dt);

        auto thetaDelta = (k1[0] + k2[0]*2 + k3[0]*2 + k4[0])*dt/6;
        auto thetaDotDelta = (k1[1] + k2[1]*2 + k3[1]*2 + k4[1])*dt/6;

        std::valarray<std::valarray<float>> result = {arg_thetas + thetaDelta, arg_thetaDots + thetaDotDelta};
        return result;

    }
    void update() {
        std::valarray<std::valarray<float>> newstate = RK4(dt, thetas, thetaDots);
        thetas = newstate[0];
        thetaDots = newstate[1];
    }
    auto get_coords() {
        std::valarray<sf::Vector2f> coords (n);

        float x_i = WIDTH/2;
        float y_i = HEIGHT/2;
        for (int i = 0; i < n; i++){
            x_i += LENGHT*sin(thetas[i]);
            y_i += LENGHT*cos(thetas[i]);

            coords[i] = sf::Vector2f(x_i, y_i);
        }
        return coords;
    }

};


int main()
{
    pendulum pend;
    std::valarray<sf::Vector2f> pend_coords (PEND_NBR);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
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
        pend.update();
        pend_coords = pend.get_coords();


        window.clear();
        //drawing part
        for(int i = 0; i < PEND_NBR; i++) {
            sf::CircleShape circle;
            circle.setRadius(5);
            circle.setOutlineThickness(0);
            circle.setPosition(pend_coords[i].x, pend_coords[i].y);
            window.draw(circle);
        }

        window.display();


        // std::cout << std::to_string(framecount) + " \n";
        framecount++;

    }

    return 0;
}