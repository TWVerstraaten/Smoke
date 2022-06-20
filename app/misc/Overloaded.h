//
// Created by pc on 20-6-22.
//

#ifndef H_APP_MISC_OVERLOADED_H
#define H_APP_MISC_OVERLOADED_H

namespace app::misc {
    // See https://en.cppreference.com/w/cpp/utility/variant/visit

    template <class... Ts>
    struct Overloaded : Ts... {
        using Ts::operator()...;
    };

    template <class... Ts>
    Overloaded(Ts...) -> Overloaded<Ts...>;
} // namespace app::misc

#endif // H_APP_MISC_OVERLOADED_H
