#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "glm/glm.hpp"

struct dist_constraint
{
    int p1;
    int p2;
    float restLenght;
};

class particle_system
{
public:
    particle_system(std::vector<glm::vec2> arg_pos
                    ,std::vector<glm::vec2> arg_acc 
                    ,glm::vec2 arg_bondry, glm::vec2 arg_gravity, int arg_subSteps
                    ,std::vector<float> arg_raduis = std::vector<float>()
                    /*,std::vector<dist_constraint> arg_distance_constraints = std::vector<dist_constraint>()*/)
    {
        m_pos = arg_pos;
        m_prev_pos = arg_pos;
        // m_distance_constraints = arg_distance_constraints;
        m_part_nbr = m_pos.size();
        m_acc = arg_acc;
        m_bondry = arg_bondry;
        m_subSteps = arg_subSteps;
        m_gravity = arg_gravity;

        m_raduis = arg_raduis;
        if(m_raduis.empty())
        {
            m_raduis.reserve(m_part_nbr+1);
            for (int i = 0; i < m_part_nbr; i++)
            {
                m_raduis.push_back(glm::linearRand(4, (int)default_raduis));
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
    
    // void draw(sf::Window window, sf::CircleShape& circle)
    // {
    //     for (int i = 0; i < m_part_nbr; i++)
    //     {
    //         circle.setPosition(m_pos[i].x - m_part_raduis, m_pos[i].y - m_part_raduis);
    //         window.draw(circle);
    //     }
    //     // for (int i = 0; i < m_distance_constraints.size(); i++)
    //     // {
    //     //     dist_constraint& c = m_distance_constraints[i];
    //     //     glm::vec2& x1 = m_pos[c.p1];
    //     //     glm::vec2& x2 = m_pos[c.p2];
    //     //     sf::Vertex line[] = {{{x1.x, x1.y}, sf::Color::White}, {{x2.x, x2.y}, sf::Color::White}};
    //     //     window.draw(line, 2, sf::Lines);
    //     // }
    // }

    void add_particle(glm::vec2 arg_position, glm::vec2 arg_acceleration, float arg_raduis = 12)
    {
        m_pos.push_back(arg_position);
        m_prev_pos.push_back(arg_position);
        m_acc.push_back(arg_acceleration);
        m_raduis.push_back(arg_raduis);
        m_part_nbr++;
    }

    std::vector<glm::vec2> m_pos;
    std::vector<glm::vec2> m_prev_pos;
    std::vector<glm::vec2> m_acc;
    std::vector<float> m_raduis;
    float default_raduis = 12;
    int m_part_nbr;



private:
    void upply_world_forces(float dt)
    {
        // gravity
        for (int i = 0; i < m_part_nbr; i++)
        {
            m_acc[i] += m_gravity*dt;
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
        for (int step = 0; step < m_subSteps; step++)
        {
        
        // window boundry
        for (int i = 0; i < m_part_nbr; i++)
        {
            // screen boundry constraint
            float& raduis = m_raduis[i];
            m_pos[i] = glm::min(glm::max(m_pos[i], {raduis, raduis}), {m_bondry.x-raduis, m_bondry.y-raduis});
        }

        // distance constaints
        // for (int i = 0; i < m_distance_constraints.size(); i++)
        // {
        //     dist_constraint& c = m_distance_constraints[i];
        //     glm::vec2& x1 = m_pos[c.p1];
        //     glm::vec2& x2 = m_pos[c.p2];
        //     glm::vec2 delta = x2 - x1;
        //     float deltaLenght = sqrt(glm::dot(delta, delta));
        //     float diff = (deltaLenght - c.restLenght)/deltaLenght;
        //     x1 += delta * 0.5f*diff;
        //     x2 -= delta * 0.5f*diff;
        // }
        
        // solve collisions : brute force
        for (int i = 0; i < m_part_nbr; i++)
        {
            for (int j = i+1; j < m_part_nbr; j++)
            {
                float raduis_sum = m_raduis[i] + m_raduis[j];

                glm::vec2 collision_axis = m_pos[i] - m_pos[j];
                float dist = glm::length(collision_axis);
                if(dist < raduis_sum)
                {
                    glm::vec2 n = glm::normalize(collision_axis);
                    float delta = raduis_sum - dist;
                    m_pos[i] += 0.5f * n*delta;
                    m_pos[j] -= 0.5f * n*delta;
                }
            }
        }
        

        }
    }

    // std::vector<dist_constraint> m_distance_constraints;
    glm::vec2 m_gravity;
    glm::vec2 m_bondry;
    int m_subSteps;

    // sf::RenderTexture window();
};
