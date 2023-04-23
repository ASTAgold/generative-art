#include "particle_system.h"

particle_system::particle_system(std::vector<glm::vec2> pos, std::vector<glm::vec2> prev_pos)
{
    this->pos = pos;
    this->prev_pos = prev_pos;
    part_nbr = pos.size();

    for (int i = 0; i < part_nbr; i++)
    {
        acc.push_back({0, 0});
    }
}

void particle_system::upply_world_forces(float dt)
{
    for (int i = 0; i < part_nbr; i++)
    {
        acc[i] += gravity*dt;
    }
    
}

void particle_system::update(float dt)
{
    // accumilate forces
    upply_world_forces(dt);

    // verlet integration
    for (int i = 0; i < part_nbr; i++)
    {
        glm::vec2 temp = pos[i];
        pos[i] += pos[i] - prev_pos[i] + acc[i]*dt*dt;
        prev_pos[i] = temp;

        acc[i] *= 0;
    }
}

// void particle_system::draw_particles(sf::Window& window, sf::CircleShape& circle)
// {
//     for (int i = 0; i < part_nbr; i++)
//     {
//         circle.setPosition(pos[i].x, pos[i].y);
//         window.draw(circle);
//     }
// }

std::vector<glm::vec2> particle_system::get_particles()
{
    return pos;
}