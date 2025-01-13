#include <array>
#include <iostream>

#include "game.h"

void Game::print_field() {
    for (char i = 0; i < 3; ++i) {
        std::cout << '|';
        for (char j = 0; j < 3; ++j) {
            std::cout << get_sign(i, j);
            std::cout << '|';
        }
        std::cout << '\n';
    }
}

GameState Game::check_win(std::pair<char, char> moved_position) {
    GameState result = DRAW;
    bool win_state = false;

    /*
     * checks:
     * for horizontal line
     * for vertical line
     * for main diagonal line
     * for minor diagonal line
     */
    if (
            (get_sign(moved_position) == get_sign((moved_position.first + 1) % 3, moved_position.second) && get_sign(moved_position) == get_sign((moved_position.first + 2) % 3, moved_position.second)) ||
            (get_sign(moved_position) == get_sign(moved_position.first, (moved_position.second + 1) % 3) && get_sign(moved_position) == get_sign(moved_position.first, (moved_position.second + 2) % 3)) ||
            ((moved_position.first == moved_position.second) && get_sign(moved_position) == get_sign((moved_position.first + 1) % 3, (moved_position.second + 1) % 3) && get_sign(moved_position) == get_sign((moved_position.first + 2) % 3, (moved_position.second + 2) % 3)) ||
            ((moved_position.first + moved_position.second == 4) && get_sign(moved_position) == get_sign((moved_position.first + 1) % 3, (moved_position.second - 1) % 3) && get_sign(moved_position) == get_sign((moved_position.first + 2) % 3, (moved_position.second - 2) % 3))
       ) {
        win_state = true;
    }

    if (win_state) {
        if (get_sign(moved_position) == 'X') result = X_WIN;
        else result = O_WIN;
    } else {
        /*
         * checks if there is a blank field to continue the game
         */
        for (auto pos = field.begin(); pos != field.end(); ++pos) {
            if (*pos == ' ') {
                result = UNKNOWN;
                break;
            }
        }
    }

    return result;
}
