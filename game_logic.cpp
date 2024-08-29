#include <vector>
#include <iostream>

#include "custom/game_logic.h"
#include "custom/utils.h"

GameLogic::GameLogic(int n_rows, int n_cols, int n_win_line)
    : nr_rows(n_rows), nr_columns(n_cols), nr_win_line(n_win_line) {

    game_data.resize(nr_rows);
    for (int i = 0; i < nr_rows; i++) {
        game_data[i].resize(nr_columns);
    }
    clear_game_data();

    win_line_data.start_row = 0;
    win_line_data.start_column = 0;
    win_line_data.stop_row = 0;
    win_line_data.stop_column = 0;
}

GameLogic::~GameLogic() {};

void GameLogic::set_cell_state(int row, int column, cell_state state) {
    game_data[row][column] = state;
}

bool GameLogic::check_win_row(int cur_row, int cur_column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_col = std::max(cur_column - nr_win_line + 1, 0); // col of first member in first group
    int next_first_col;
    cell_state cur_state = game_data[cur_row][cur_column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    while (group_first_col + nr_win_line - 1 < nr_columns) { // we check that we are still in boundries
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[cur_row][group_first_col + i] != cur_state)
            {
                next_first_col = group_first_col + i + 1; // we already know the column that doesn t match
                jump = true;
            }
        }
    
        if (jump == false) { // win line
            // we save row and column for win line endpoints
            win_line_data.start_row = cur_row;
            win_line_data.start_column = group_first_col;
            win_line_data.stop_row = cur_row;
            win_line_data.stop_column = group_first_col + nr_win_line - 1;
       
            return true;
        } else {
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win_column(int cur_row, int cur_column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_row = std::max(cur_row - nr_win_line + 1, 0); // row of first member in first group
    int next_first_row;
    cell_state cur_state = game_data[cur_row][cur_column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    while (group_first_row + nr_win_line - 1 < nr_rows) { // we check that we are still in boundries
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[group_first_row + i][cur_column] != cur_state)
            {
                next_first_row = group_first_row + i + 1; // we already know the column that doesn t match
                jump = true;
            }
        }
    
        if (jump == false) { // win line
            // we save row and column for win line endpoints
            win_line_data.start_row = group_first_row;
            win_line_data.start_column = cur_column;    
            win_line_data.stop_row = group_first_row + nr_win_line - 1;
            win_line_data.stop_column = cur_column;
            return true;
        } else {
            group_first_row = next_first_row;
        }
    }

    return false;
}

bool GameLogic::check_win_diag1(int cur_row, int cur_column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = cur_row - std::max(cur_row - nr_win_line + 1, 0);
    int max_col_offset = cur_column - std::max(cur_column - nr_win_line + 1, 0);
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row, group_first_col;
    int next_first_row, next_first_col;
    cell_state cur_state = game_data[cur_row][cur_column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    group_first_row = cur_row - offset;
    group_first_col = cur_column - offset;

    while (group_first_row + nr_win_line - 1 < nr_rows && group_first_col + nr_win_line - 1 < nr_columns) {
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[group_first_row + i][group_first_col + i] != cur_state)
            {
                next_first_row = group_first_row + i + 1;
                next_first_col = group_first_col + i + 1;
                jump = true;
            }
        }
    
        if (jump == false) {
            win_line_data.start_row = group_first_row;
            win_line_data.start_column = group_first_col;    
            win_line_data.stop_row = group_first_row + nr_win_line - 1;
            win_line_data.stop_column = group_first_col + nr_win_line - 1;
            return true;
        } else {
            group_first_row = next_first_row;
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win_diag2(int cur_row, int cur_column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = cur_row - std::max(cur_row - nr_win_line + 1, 0);
    // we start from top right corner so we adjust the computations
    int max_col_offset = std::min(cur_column + nr_win_line - 1, nr_columns) - cur_column;
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row, group_first_col;
    int next_first_row, next_first_col;
    cell_state cur_state = game_data[cur_row][cur_column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    group_first_row = cur_row - offset;
    group_first_col = cur_column + offset;

    while (group_first_row + nr_win_line - 1 < nr_rows && group_first_col - nr_win_line + 1 >= 0) {
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[group_first_row + i][group_first_col - i] != cur_state)
            {
                next_first_row = group_first_row + i + 1;
                next_first_col = group_first_col - i - 1;
                jump = true;
            }
        }
    
        if (jump == false) {
            win_line_data.start_row = group_first_row;
            win_line_data.start_column = group_first_col;    
            win_line_data.stop_row = group_first_row + nr_win_line - 1;
            win_line_data.stop_column = group_first_col - nr_win_line + 1;
            return true;
        } else {
            group_first_row = next_first_row;
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win(int cur_row, int cur_column) {
    ////
    std::cout << "ROW: " << check_win_row(cur_row, cur_column)
            << "| COL: " << check_win_column(cur_row, cur_column) 
            << "| DIAG1: " << check_win_diag1(cur_row, cur_column)
            << "| DIAG2: " << check_win_diag2(cur_row, cur_column) << "\n";
    ////

    return check_win_row(cur_row, cur_column) || check_win_column(cur_row, cur_column)
        || check_win_diag1(cur_row, cur_column) || check_win_diag2(cur_row, cur_column);
}

grid_line_data GameLogic::get_win_line_data() {
    return win_line_data;
}

void GameLogic::clear_game_data() {
    for (int i = 0; i < nr_rows; i++) {
        for (int j = 0; j < nr_columns; j++) {
            game_data[i][j] = CELL_EMPTY;
        }
    }
}

Player::Player(player_type t, cell_state s, GameLogic* gl, GameGrid* gg)
    : type(t), used_symbol(s), game_logic_p(gl), game_grid_p(gg) {};

Player::~Player() {};

Human::Human(cell_state s, GameLogic* gl, GameGrid* gg)
    : Player(HUMAN, s, gl, gg) {};

void Human::human_action() {

}

void Human::do_next_action() {
    human_action();
}

Robot::Robot(cell_state s, GameLogic* gl, GameGrid* gg)
    : Player(ROBOT, s, gl, gg) {};

Robot::~Robot() {

};

void Robot::robot_action() {

}

void Robot::do_next_action() {
    robot_action();
}