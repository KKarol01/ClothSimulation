#pragma once

#include <glm/glm.hpp>

namespace ph {
    class Camera {
        float mx{0.f}, my{0.f};

      public:
        Camera();

        float yaw{0.f}, pitch{0.f}, sensitivity{0.2f};
        glm::vec3 pos{0.f, 0.5f, 0.f};
        glm::vec3 right{1.f, 0.f, 0.f}, forward{0.f, 0.f, -1.f}, up{0.f, 1.f, 0.f};

        void Update();
        glm::mat4 ViewMatrix() const;
    };
} // namespace ph
