//
// Created by pc on 22-11-21.
//

#include "State.h"

namespace app {
    std::unique_ptr<State> State::s_state;

    State& State::get() {
        if (not s_state)
            s_state = std::unique_ptr<State>(new State());

        return *s_state;
    }

    void State::toggle_paused() {
        m_paused = !m_paused;
    }

    bool State::paused() const {
        return m_paused;
    }
} // namespace app
