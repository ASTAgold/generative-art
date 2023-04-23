#pragma once
#include <SFML/Graphics.hpp>
#include "../../glm/glm.hpp"
#include <vector>
#include <algorithm> 

class particle_system
{
private:
    std::vector<glm::vec2> pos;
    std::vector<glm::vec2> prev_pos;
    std::vector<glm::vec2> acc;
    glm::vec2 gravity = {0, 9.8};

    void upply_world_forces(float dt);
public:
    int part_nbr = 0;

    particle_system(std::vector<glm::vec2> pos, std::vector<glm::vec2> prev_pos);

    void update(float dt);
    // void draw_particles(sf::Window window, sf::CircleShape& circle);
    std::vector<glm::vec2> get_particles();
};