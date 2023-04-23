#include <SFML/Graphics.hpp>
#include "../glm/ext.hpp"
#include <stdio.h>

#define CURRENT

int framecount = 0;
int WIDTH = 650;
int HEIGHT = 450;


int main()
{
#ifdef TESTING
    sf::CircleShape testCircle(50);
    testCircle.setFillColor({255, 0, 0, 255});
#endif

    //////// SETUP
    sf::Clock clock;
    sf::Time currentTime = clock.getElapsedTime();
    float dt;
    unsigned int fps; 
#ifdef CURRENT
    sf::Texture current;
    current.loadFromFile("resources/canvas.png");
#endif
    sf::RenderTexture texture;
    texture.create(WIDTH, HEIGHT);

	sf::Sprite sprite;

    sf::Shader coreShader;
    coreShader.loadFromFile("shader.frag", sf::Shader::Fragment);
    coreShader.setUniform("u_resolution", sf::Vector2f(WIDTH, HEIGHT));

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


        dt = (clock.getElapsedTime().asSeconds() - currentTime.asSeconds());
        currentTime = clock.getElapsedTime();
        fps = (int)roundf(1/dt);

        coreShader.setUniform("u_time", clock.getElapsedTime().asSeconds());
        coreShader.setUniform("u_mouse", sf::Vector2f(sf::Mouse::getPosition(window)));
#ifdef CURRENT
        coreShader.setUniform("u_current", sf::Shader::CurrentTexture);
        coreShader.setUniform("u_currentSize", sf::Vector2f(WIDTH, HEIGHT));
#endif

        //////// UPDATE

        texture.clear({255, 255, 255, 255});
        auto tmpSprite = sf::Sprite(current);
        tmpSprite.setScale(WIDTH / (float)current.getSize().x, HEIGHT / (float)current.getSize().y);
        texture.draw(tmpSprite);

        texture.draw(sf::Sprite(texture.getTexture()), &coreShader);

        window.clear();
        //////// DRAW

        sprite.setTexture(texture.getTexture());
        window.draw(sprite);

#ifdef TESTING
        testCircle.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
        window.draw(testCircle);
#endif

        window.display();

        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}