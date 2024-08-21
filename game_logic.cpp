#include <vector>

#include "custom/game_logic.h"
#include "custom/utils.h"

GameLogic::GameLogic(int n_rows, int n_cols, int n_cross_win)
    : nr_rows(n_rows), nr_columns(n_cols), nr_cross_win(n_cross_win) {

    game_data.resize(nr_rows);
    for (int i = 0; i < nr_rows; i++) {
        game_data[i].resize(nr_columns);
    }
    clear_game_data();
}

GameLogic::~GameLogic() {};

void GameLogic::clear_game_data() {
    for (int i = 0; i < nr_rows; i++) {
        for (int j = 0; j < nr_columns; j++) {
            game_data[i][j] = CELL_EMPTY;
        }
    }
}

Player::Player(player_type t, symbol s, GameLogic* gl, GameGrid* gg)
    : type(t), used_symbol(s), game_logic_p(gl), game_grid_p(gg) {};

Player::~Player() {};

Human::Human(symbol s, GameLogic* gl, GameGrid* gg)
    : Player(HUMAN, s, gl, gg) {};

void Human::human_action() {

}

void Human::do_next_action() {
    human_action();
}

Robot::Robot(symbol s, GameLogic* gl, GameGrid* gg)
    : Player(ROBOT, s, gl, gg) {};

Robot::~Robot() {

};

void Robot::robot_action() {

}

void Robot::do_next_action() {
    robot_action();
}