#pragma once
#include <functional>

#include "../hw_ui/controller.hpp"

namespace ph {
    struct UpdateCommand {
        typedef std::function<void(float)> ptr;
        ptr _ptr;

        UpdateCommand() = default;
        explicit UpdateCommand(ptr _ptr) : _ptr(_ptr) {}
        virtual void operator()(float dt) const { _ptr(dt); }
        virtual UpdateCommand *clone() const { return new UpdateCommand(*this); };
    };

    struct UpdateControllerCommand : UpdateCommand {
        Controller *c;
        explicit UpdateControllerCommand(Controller *c) : UpdateCommand(nullptr), c(c) {}
        virtual void operator()(float dt) const override { c->Update(dt); }
        virtual UpdateControllerCommand *clone() const override { return new UpdateControllerCommand(*this); }
    };
} // namespace ph