#pragma once
#include "ball.hpp"
#include "../shader/shader.hpp"
#include "../buffer/buffer.hpp"
#include <vector>
#include <random>
#include <memory>
#include <map>

class Cloth {
    std::vector<Ball> balls;
    std::unique_ptr<Shader> shader, shader_sticks;
    std::unique_ptr<Shader> compute_shader, compute_physics;

    uint32_t vao, vbo, ebo, vbosmooth;
    uint32_t tex;

    size_t indice_count{0};
    size_t vbo_size, ebo_size, vbo_smooth_size;
    float row_dist, col_dist;
    glm::vec3 gravity_force{0.f, -0.09f , 0.f};


    struct Stick {
        Stick(Ball &a, Ball &b, float dist) : a(a), b(b), dist(dist), inv_dist(1.0f / dist) {}

        Ball &a, &b;
        float dist, inv_dist;
    };
    std::vector<Stick> sticks;

  public:
    size_t rows{0}, cols{0};

    Cloth(size_t rows, size_t cols);
    void Draw(float delta_time);
    void AddBall(const glm::vec3 &pos);
    void ConnectBalls(unsigned a, unsigned b, float dist);
    void SetBallPinnedStatus(unsigned a, bool pinned);

    void _RECOMPILE();

  private:
    void Update(float delta_time);
    void ConstrainMovement(Ball &b, float delta_time);
    std::vector<glm::vec3> CalculateNormals();
};