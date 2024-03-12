#include "camera.hpp"
#include "../engine.hpp"

#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

ph::Camera::Camera() { auto [mx, my] = Engine::GetWindow()->GetCursorPos(); }

void ph::Camera::Update() {
    const auto [tmx, tmy] = Engine::GetWindow()->GetCursorPos();

    const auto look = Engine::GetController()->look();
    const auto dx   = (-look.x) * sensitivity * 10.f;
    const auto dy   = (-look.y) * sensitivity * 10.f;

    mx = tmx;
    my = tmy;

    yaw += dx;
    pitch = glm::clamp(pitch + dy, -89.9f, 89.0f);

    glm::quat rotation;
    rotation = glm::angleAxis(glm::radians(yaw), glm::vec3{0.f, 1.f, 0.f});
    rotation *= glm::angleAxis(glm::radians(pitch), glm::vec3{1.f, 0.f, 0.f});

    forward = glm::vec3{rotation * glm::vec3{0.f, 0.f, -1.f}};
    right   = glm::cross(forward, up);

    const auto move                      = Engine::GetController()->move() * 0.02f;
    const auto horizontal_plane_movement = glm::vec3{1.f, 0.f, 1.f};
    pos += right * move.x * horizontal_plane_movement;
    pos += up * move.y;
    pos += forward * -move.z * horizontal_plane_movement;
}

glm::mat4 ph::Camera::ViewMatrix() const { return glm::lookAt(pos, pos + forward, up); }
