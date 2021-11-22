//
// Created by pc on 22-11-21.
//

#ifndef H_APP_STATE_H
#define H_APP_STATE_H

#include <memory>

namespace app {

    class State {

      public:
        void                        set_paused(bool is_paused);
        void                        toggle_paused();
        [[nodiscard]] bool          paused() const;
        [[nodiscard]] static State& get();

      private:
        State() = default;

        bool                          m_paused = false;
        static std::unique_ptr<State> s_state;
    };

} // namespace app

#endif // H_APP_STATE_H
