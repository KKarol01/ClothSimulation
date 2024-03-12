#include "../engine.hpp"
#include <GLFW/glfw3.h>
#include "controller.hpp"
#include <glm/gtx/norm.hpp>
#include "../update_command/update_command.hpp"

static void ct(float dt) { ph::Engine::GetController()->Update(dt); }

ph::Controller::Controller() { _callback_id = Engine::OnUpdateSubscribe(ph::UpdateControllerCommand{this}); }

ph::Controller::~Controller() { Engine::OnUpdateUnSubscribe(_callback_id); }

ph::Gamepad::Gamepad() : Controller() {
    const char *mappings = R"(
            78696e70757401000000000000000000
            XInput Gamepad (GLFW)
            platform:Windows

            leftx:a0
            lefty:a1
            rightx:a2
            righty:a3
            lefttrigger:a4
            rightrigger:a5

            a:b0
            b:b1
            c:b2
            d:b3
            lefthshoulder:b4
            rightshoulder:b5
            leftstick:b6
            rightstick:b7
            start:b8
            back:b9
            
            dpup:h0.1
            dpright:h0.2
            dpdown:h0.4
            dpleft:h0.8
        )";
    glfwUpdateGamepadMappings(mappings);
    state = new GLFWgamepadstate{};
}

ph::Gamepad::~Gamepad() { delete state; }

static constexpr float deadzone = 0.4f;

void ph::Gamepad::Update(float delta_time) {

    auto &state = *this->state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
    glm::vec2 left_analog{state.axes[0], state.axes[1]};
    glm::vec2 right_analog{state.axes[2], state.axes[3]};

    if (glm::length2(left_analog) <= deadzone * deadzone) left_analog = {0.f, 0.f};
    if (glm::length2(right_analog) <= deadzone * deadzone) right_analog = {0.f, 0.f};

    buttons[BUTTON::A]     = state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS;
    buttons[BUTTON::B]     = state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS;
    buttons[BUTTON::X]     = state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS;
    buttons[BUTTON::Y]     = state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS;
    buttons[BUTTON::LB]    = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;
    buttons[BUTTON::RB]    = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;
    buttons[BUTTON::DU]    = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS;
    buttons[BUTTON::DR]    = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS;
    buttons[BUTTON::DL]    = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS;
    buttons[BUTTON::DD]    = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS;
    buttons[BUTTON::LS]    = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS;
    buttons[BUTTON::RS]    = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS;
    buttons[BUTTON::START] = state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS;
    buttons[BUTTON::BACK]  = state.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS;

    analogs[BUTTON::LA]  = left_analog;
    analogs[BUTTON::RA]  = right_analog;
    triggers[BUTTON::LT] = state.axes[4];
    triggers[BUTTON::RT] = state.axes[5];
}

glm::vec2 ph::Gamepad::look() const { return analogs.at(BUTTON::RA); }

glm::vec3 ph::Gamepad::move() const {
    return {analogs.at(BUTTON::LA)[0],
            (buttons.at(BUTTON::A) == 1) ? 1.f : (buttons.at(BUTTON::B) == 1 ? -1.f : 0.f),
            analogs.at(BUTTON::LA)[1]};
}

ph::KeyboardAndMouse::KeyboardAndMouse() : Controller() {
    auto [cx, cy]         = ph::Engine::GetWindow()->GetCursorPos();
    prev_cursor_positon   = {cx, cy};
    cursor_position_delta = {0.f, 0.f};
}

ph::KeyboardAndMouse::~KeyboardAndMouse() {}

glm::vec2 ph::KeyboardAndMouse::look() const { return cursor_position_delta; }

glm::vec3 ph::KeyboardAndMouse::move() const {
    auto window = ph::Engine::GetWindow()->GLFWPTR();

    const auto A     = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    const auto D     = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    const auto W     = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    const auto S     = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    const auto SPACE = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    const auto SHIFT = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    return glm::vec3{
        D     ? 1.f : A     ? -1.f : 0.f,
        SPACE ? 1.f : SHIFT ? -1.f : 0.f,
        W     ?-1.f : S     ?  1.f : 0.f};
}

void ph::KeyboardAndMouse::Update(float delta_time) {
    auto [cx, cy]         = ph::Engine::GetWindow()->GetCursorPos();
    cursor_position_delta = (glm::vec2{cx, cy} - prev_cursor_positon) * 0.1f;
    prev_cursor_positon   = glm::vec2{cx, cy};
}
