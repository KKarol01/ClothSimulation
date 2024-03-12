#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

struct GLFWgamepadstate;

namespace ph {
    enum class INPUT_ACTION { LOOK_RIGHT, LOOK_UP, MOVE_RIGHT, MOVE_UP, MOVE_FORWARD };
    class Controller {
      public:
        Controller();
        virtual ~Controller();

        virtual glm::vec2 look() const        = 0;
        virtual glm::vec3 move() const        = 0;
        virtual void Update(float delta_time) = 0;

      private:
        size_t _callback_id = 0;
    };

    class Gamepad : public Controller {
      public:
        Gamepad();
        virtual ~Gamepad();
        virtual void Update(float delta_time) override;
        virtual glm::vec2 look() const override;
        virtual glm::vec3 move() const override;

      private:
        enum class BUTTON : uint8_t { A, B, X, Y, LS, RS, LB, RB, LA, RA, LT, RT, DU, DR, DL, DD, START, BACK };
        GLFWgamepadstate *state;
        std::unordered_map<BUTTON, bool> buttons;
        std::unordered_map<BUTTON, glm::vec2> analogs;
        std::unordered_map<BUTTON, float> triggers;
    };

    class KeyboardAndMouse : public Controller {
      public:
        KeyboardAndMouse();
        virtual ~KeyboardAndMouse();

        virtual glm::vec2 look() const override;
        virtual glm::vec3 move() const override;
        virtual void Update(float delta_time) override;

      private:
        glm::vec2 prev_cursor_positon;
        glm::vec2 cursor_position_delta;
    };
} // namespace ph