#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"
#include "glm/gtx/fast_square_root.hpp"
#include "include/FastNoiseLite.h"
#include <stdio.h>
#include <vector>
#include <algorithm>


int framecount = 0;
const int WIDTH = 550;
const int HEIGHT = 450;
const int PART_RADUIS = 3;
const int PART_RESOLUTION = 5;

float clothWidth = 300;
float clothHeight = 250;
int cols = 15;
int rows = 15;

float lenghtX = clothWidth/cols;
float lenghtY = clothHeight/rows;

float WIND_FORCE = 10;

// [0,1]
int random(int max)
{
    return rand() % max;
}

struct dist_constraint
{
    int p1;
    int p2;
    float restLenght;
};

struct particle_system
{
    std::vector<glm::vec2> m_pos;
    std::vector<glm::vec2> m_prev_pos;
    std::vector<glm::vec2> m_acc;
    std::vector<dist_constraint> m_distance_constraints;
    std::vector<float> m_invmass;
    glm::vec2 gravity = {0, 4};
    int m_substeps = 8;
    int m_part_nbr;

    particle_system(std::vector<glm::vec2> arg_pos, std::vector<glm::vec2> arg_prev_pos, std::vector<float> arg_invmass, std::vector<dist_constraint> arg_distance_constraints)
    {
        m_pos = arg_pos;
        m_prev_pos = arg_prev_pos;
        m_part_nbr = m_pos.size();
        m_invmass = arg_invmass;

        for (int i = 0; i < m_part_nbr; i++)
        {
            m_acc.push_back({0, 0});
        }

        //constraints
        m_distance_constraints = arg_distance_constraints;

        // if(arg_distance_constraints.empty())
        // {
        //     for (int i = 0; i < m_part_nbr-1; i++)
        //     {
        //         m_distance_constraints.push_back({i, i+1, random(200)});
        //     }
        // }else 
        // {
        //     m_distance_constraints = arg_distance_constraints;
        // }
    }

    void upply_world_forces(FastNoiseLite& arg_noise ,float dt,float t)
    {
        // gravity
        for (int i = 0; i < m_part_nbr; i++)
        {
            m_acc[i] += gravity*m_invmass[i]*dt;
        }

        //perlin noise wind
        for (int i = 0; i < m_part_nbr; i++)
        {
            glm::vec2& pos = m_pos[i];
            glm::vec2 force(arg_noise.GetNoise(pos.x, t)*WIND_FORCE, arg_noise.GetNoise(pos.x, t)*WIND_FORCE);
            m_acc[i] += force*m_invmass[i]*dt;
        }
        
        
    }

    void verlet(float dt)
    {
        // verlet integration
        for (int i = 0; i < m_part_nbr; i++)
        {
            glm::vec2 m_vel = m_pos[i] - m_prev_pos[i];
            m_prev_pos[i] = m_pos[i];
            m_pos[i] += m_vel + m_acc[i]*dt*dt;

            m_acc[i] *= 0;
        }
    }

    void solve_constraints()
    {
        for (int step = 0; step < m_substeps; step++)
        {
        
            // window boundry
            for (int i = 0; i < m_part_nbr; i++)
            {
                // screen boundry constraint
                m_pos[i] = glm::min(glm::max(m_pos[i], {PART_RADUIS, PART_RADUIS}), {WIDTH-PART_RADUIS, HEIGHT-PART_RADUIS});
            }

            // distance constaints
            for (int i = 0; i < m_distance_constraints.size(); i++)
            {
                dist_constraint& c = m_distance_constraints[i];
                glm::vec2& x1 = m_pos[c.p1];
                glm::vec2& x2 = m_pos[c.p2];
                float& invm1 = m_invmass[c.p1];
                float& invm2 = m_invmass[c.p2];
                glm::vec2 delta = x2 - x1;
                float deltaLenght = glm::sqrt(glm::dot(delta, delta));
                float diff = (deltaLenght - c.restLenght)/(deltaLenght * (invm1 + invm2));
                x1 += delta * invm1*diff;
                x2 -= delta * invm2*diff;
            }
        
        }
    }

    void update(FastNoiseLite& arg_noise, float dt, float t)
    {
        //accumilate forces
        upply_world_forces(arg_noise, dt, t);
        verlet(dt);
        solve_constraints();
    }
};

int main()
{
    srand(time(NULL));
    //////// SETUP
    sf::Clock timer;

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // CLOTH MAKING
    std::vector<glm::vec2> particles;
    std::vector<float> invmass;
    // std::vector<glm::vec2> prev_particles;
    std::vector<dist_constraint> distance_constraints;
    particles.reserve(cols*rows+1);
    distance_constraints.reserve(cols*rows*2);
    for (int j = 0; j < rows; j++)
    {
        for (int i = 0; i < cols; i++)
        {
            particles.push_back({i*lenghtX + (WIDTH - clothWidth)/2, j*lenghtY + (HEIGHT - clothHeight)/2});
            invmass.push_back(1);
        }
    }
    // invmass[0] = 0;
    // invmass[floor(cols/2)] = 0;
    // for (int i = 0; i < cols; i++)
    // {
    //     invmass[i] = (i % 2 == 1) ? 0.00000000000000001: 1;
    // }
    
    float diagonale = sqrt(lenghtX*lenghtX + lenghtY*lenghtY);
    for (int i = 0; i < cols-1; i++)
    {    
        for (int j = 0; j < rows-1; j++)
        {
            int index = i + j*cols;

            distance_constraints.push_back({index, i+1 + j*cols, lenghtX});
            distance_constraints.push_back({index, i + (j+1)*cols, lenghtY});
            distance_constraints.push_back({index, i+1 + (j+1)*cols, diagonale});
            
            // if(i != cols-1)
            // {
            //     distance_constraints.push_back({index, i + (j+1)*rows, 30});
            //     distance_constraints.push_back({index, i+1 + (j+1)*rows, 30});
            // }
        }
    }
    particle_system PARTICLE_SYSTEM(particles, particles, invmass, distance_constraints);

    sf::CircleShape CIRCLE(PART_RADUIS, PART_RESOLUTION);
    CIRCLE.setFillColor(sf::Color::White);
    CIRCLE.setOutlineThickness(0);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "dop :)");
    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        //////// DRAW
        for (int i = 0; i < PARTICLE_SYSTEM.m_part_nbr; i++)
        {
            CIRCLE.setPosition(PARTICLE_SYSTEM.m_pos[i].x - PART_RADUIS, PARTICLE_SYSTEM.m_pos[i].y - PART_RADUIS);
            window.draw(CIRCLE);
        }
        for (int i = 0; i < PARTICLE_SYSTEM.m_distance_constraints.size(); i++)
        {
            dist_constraint& c = PARTICLE_SYSTEM.m_distance_constraints[i];
            glm::vec2& x1 = PARTICLE_SYSTEM.m_pos[c.p1];
            glm::vec2& x2 = PARTICLE_SYSTEM.m_pos[c.p2];
            sf::Vertex line[] = {{{x1.x, x1.y}, sf::Color::White}, {{x2.x, x2.y}, sf::Color::White}};
            window.draw(line, 2, sf::Lines);
        }
        

        window.display();

        //////// UPDATE
        timer.restart();
        PARTICLE_SYSTEM.update(noise, 0.4, (float)framecount);
        sf::Time elapced = timer.getElapsedTime();
        printf("%d\n", elapced.asMicroseconds());

        // RECORDING
        // sf::Texture texture;
        // texture.create(window.getSize().x, window.getSize().y);
        // texture.update(window);
        // std::string filename = std::to_string(framecount);
        // texture.copyToImage().saveToFile("movio_5/" + filename + ".png");


        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}