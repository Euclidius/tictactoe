#pragma once
#include <array>
#include <iostream>

/**
 * !!!
 * there is a bug: when u send string instead of a number everything falls
 */


/*
 * State of the tame:
 * X_WIN - win of the first player
 * Y_WIN - win of the second player
 * DRAW - the field is full and nobody wins
 * UNKNOWN - the game is not ended yet
 */

enum GameState {
    X_WIN,
    O_WIN,
    DRAW,
    UNKNOWN,
};

class Game {
public:
    Game(): 
        field(std::array<char, 9>())
    {
        for (auto pos = this->field.begin(); pos != this->field.end(); ++pos) {
            *pos = ' ';
        }
    };

    char get_sign(std::pair<char, char> coords) {
        return this->field[coords.first * 3 + coords.second];
    }

    char get_sign(char x, char y) {
        return this->field[x * 3 + y];
    }

    void set_sign(std::pair<char, char> coords, char sign) {
        this->field[coords.first * 3 + coords.second] = sign;
    }

    /*
     * checks the game state
     * moved_position used to optimise the checking
     */
    GameState check_win(std::pair<char, char> moved_position);

    std::string get_field_as_string() {
        std::string res;
        for (int i = 0; i < 9; ++i) {
            res += this->field[i];
        }
        return res;
    }

    /*
     * field[postion] garantees to bee free
     */
    void make_move(std::pair<char, char>position, char move_sign) {
        set_sign(position, move_sign);
    }

    void print_field();

private:
    std::array<char, 9> field;
};
