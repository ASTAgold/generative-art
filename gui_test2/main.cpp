#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <stdio.h>


int framecount = 0;
int WIDTH = 600;
int HEIGHT = 600;

// CIRCLE PARAMS
int radius = 80;
int resolution = 8;

// GUI PARAMS
int offset = 5;
char* buttonWidth = "10%";
char* buttonHeight = "5%";
char* sliderWidth = "95%";
int sliderHeight = 12;

int rediusMax = WIDTH/2;
int rediusMin = 0;
int resolutionMax = 64;
int resolutionMin = 0;

// TO DO
// void set_value(int& _value, tgui::Gui& _gui) {_value = _gui.get<tgui::ChildWindow>("debugWindow")->get<tgui::Slider>("slider")->getValue();}

int main()
{
    //////// SETUP

    sf::CircleShape circle(radius);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(60);

    tgui::GuiSFML gui{window};
    int y = 0;

    auto debugWindow = tgui::ChildWindow::create();
    debugWindow->setClientSize({250, 150});
    debugWindow->setPosition(0, 0);
    debugWindow->setTitle("debug window");
    gui.add(debugWindow, "debugWindow");

    auto radiusSlider = tgui::Slider::create();
    radiusSlider->setPosition(0, y);
    y += sliderHeight + offset;
    radiusSlider->setSize(sliderWidth, 14);
    radiusSlider->setMinimum(rediusMin);
    radiusSlider->setMaximum(rediusMax);
    radiusSlider->setStep(rediusMax/40);
    radiusSlider->onValueChange([&]{radius = gui.get<tgui::ChildWindow>("debugWindow")->get<tgui::Slider>("radiusSlider")->getValue();});
    debugWindow->add(radiusSlider, "radiusSlider");

    auto resolutionSlider = tgui::Slider::create();
    resolutionSlider->setPosition(0, y);
    y += sliderHeight + offset;
    resolutionSlider->setSize(sliderWidth, 14);
    resolutionSlider->setMinimum(resolutionMin);
    resolutionSlider->setMaximum(resolutionMax);
    resolutionSlider->setStep(1);
    resolutionSlider->onValueChange([&]{resolution = gui.get<tgui::ChildWindow>("debugWindow")->get<tgui::Slider>("resolutionSlider")->getValue();});
    debugWindow->add(resolutionSlider, "resolutionSlider");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        //////// UPDATE
        circle.setRadius(radius);
        circle.setPosition(WIDTH/2 - radius, HEIGHT/2 - radius);
        circle.setPointCount(resolution);

        window.clear();
        //////// DRAW
        window.draw(circle);

        gui.draw();

        window.display();

        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}