#pragma once

#include <glm/glm.hpp>

class Ball {
  public:
    Ball(const glm::vec3 &position) : position(position), prev_position(position) {}
    Ball(const glm::vec3 &position, float mass) : Ball(position) { this->mass = mass; }

    

    glm::vec3 position{0.f}, prev_position{0.f};
    glm::vec3 force{0.f};
    float mass{1.f};
    bool pinned = false;

    inline glm::vec3 velocity() const { return position - prev_position + force; }
    inline void apply_force(const glm::vec3 &f) { force += f; }
};