#include <SFML/Graphics.hpp>
#include "../glm/glm.hpp"
#include <stdio.h>
#include <vector>
#include <algorithm>


int framecount = 0;
const int WIDTH = 600;
const int HEIGHT = 600;
const int PART_RADUIS = 10;

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
    glm::vec2 gravity = {0, 9.8};
    int m_substeps = 4;
    int m_part_nbr;

    particle_system(std::vector<glm::vec2> arg_pos, std::vector<glm::vec2> arg_prev_pos, std::vector<dist_constraint> arg_distance_constraints)
    {
        m_pos = arg_pos;
        m_prev_pos = arg_prev_pos;
        m_distance_constraints = arg_distance_constraints;
        m_part_nbr = m_pos.size();

        for (int i = 0; i < m_part_nbr; i++)
        {
            m_acc.push_back({0, 0});
        }

        //constraints
        // for (int i = 0; i < m_part_nbr-1; i++)
        // {
        //     m_distance_constraints.push_back({i, i+1, random(200)});
        // }
    }

    void upply_world_forces(float dt)
    {
        // gravity
        for (int i = 0; i < m_part_nbr; i++)
        {
            m_acc[i] += gravity*dt;
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
            m_pos[i] = glm::min(glm::max(m_pos[i], {0, 0}), {WIDTH, HEIGHT});
        }

        // distance constaints
        for (int i = 0; i < m_distance_constraints.size(); i++)
        {
            dist_constraint& c = m_distance_constraints[i];
            glm::vec2& x1 = m_pos[c.p1];
            glm::vec2& x2 = m_pos[c.p2];
            glm::vec2 delta = x2 - x1;
            float deltaLenght = sqrt(glm::dot(delta, delta));
            float diff = (deltaLenght - c.restLenght)/deltaLenght;
            x1 += delta * 0.5f*diff;
            x2 -= delta * 0.5f*diff;
        }
        
        }
    }

    void update(float dt)
    {
        //accumilate forces
        upply_world_forces(dt);
        verlet(dt);
        solve_constraints();
    }
};

int pts_nbr = 20;

int main()
{
    srand(time(NULL));
    //////// SETUP
    std::vector<glm::vec2> particles = {{random(WIDTH),random(HEIGHT)},{random(WIDTH),random(HEIGHT)},{random(WIDTH),random(HEIGHT)},{random(WIDTH),random(HEIGHT)}};
    std::vector<glm::vec2> prev_particles = particles;
    particles.reserve(pts_nbr+1);
    // for (int i = 0; i < pts_nbr; i++)
    // {
    //     particles.push_back({random(WIDTH), random(HEIGHT)});
    //     prev_particles.push_back(particles[i]  + glm::vec2(random(10), random(10)));
    // }
    particle_system PARTICLE_SYSTEM(particles, prev_particles, {{0,1,100},{1,2,100},{2,3,100},{3,0,100},{0,2,sqrt(2)*100}});

    sf::CircleShape CIRCLE(PART_RADUIS, 8);
    CIRCLE.setFillColor(sf::Color::White);
    CIRCLE.setOutlineThickness(0);

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

        //////// UPDATE
        PARTICLE_SYSTEM.update(0.2);

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

        // printf("%d\n", framecount);
        framecount++;
    }
    return 0;
}