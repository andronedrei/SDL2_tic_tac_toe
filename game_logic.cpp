#include <vector>
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL_image.h>

#include "custom/game_logic.h"
#include "custom/game_interface.h"
#include "custom/utils.h"

GameLogic::GameLogic(int n_rows, int n_cols, int n_win_line)
    : nr_rows(n_rows), nr_columns(n_cols), nr_win_line(n_win_line) {

    game_data.resize(nr_rows);
    for (int i = 0; i < nr_rows; i++) {
        game_data[i].resize(nr_columns);
    }
    clear_game_data();

    cur_pos = {0, 0};
    nr_used_cells = 0;

    win_line_data.start_cell.row = 0;
    win_line_data.start_cell.column = 0;
    win_line_data.stop_cell.row = 0;
    win_line_data.stop_cell.column = 0;
}

GameLogic::~GameLogic() {};

void GameLogic::DEBUG_func() {
    std::cout << "_____\nLOGIC DEBUG:\n";
    std::cout << "NR CELLS: Nr rows: " << nr_rows << " Nr columns: " << nr_columns << "\n";
    std::cout << "NR_WIN_LINE: " << nr_win_line << "\n";
    std::cout << "CUR POS: (" << cur_pos.row << "," << cur_pos.column << ")\n";
    for (int i = 0; i < nr_rows; i++) {
        for (int j = 0; j < nr_columns; j++) {
            std::cout << game_data[i][j] << " ";
        }
        std::cout << "\n";
    }
}

cell_state GameLogic::get_cell_state(cell_pos pos) {
    return game_data[pos.row][pos.column];
}

int GameLogic::get_nr_rows() {
    return nr_rows;
}

int GameLogic::get_nr_columns() {
    return nr_columns;
}

int GameLogic::get_nr_used_cells() {
    return nr_used_cells;
}

void GameLogic::set_cell_state(cell_pos pos, cell_state state) {
    if (game_data[pos.row][pos.column] == CELL_EMPTY &&  state != CELL_EMPTY) {
        nr_used_cells++;
    }

    if (game_data[pos.row][pos.column] != CELL_EMPTY &&  state == CELL_EMPTY) {
        nr_used_cells--;
    }

    game_data[pos.row][pos.column] = state;

    cur_pos.row = pos.row;
    cur_pos.column = pos.column;
}

bool GameLogic::check_win_row(int row, int column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_col = std::max(column - nr_win_line + 1, 0); // col of first member in first group
    int next_first_col;
    cell_state cur_state = game_data[row][column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    while (group_first_col + nr_win_line - 1 < nr_columns) { // we check that we are still in boundries
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[row][group_first_col + i] != cur_state)
            {
                next_first_col = group_first_col + i + 1; // we already know the column that doesn t match
                jump = true;
            }
        }
    
        if (jump == false) { // win line
            // we save row and column for win line endpoints
            win_line_data.start_cell.row = row;
            win_line_data.start_cell.column = group_first_col;
            win_line_data.stop_cell.row = row;
            win_line_data.stop_cell.column = group_first_col + nr_win_line - 1;
       
            return true;
        } else {
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win_column(int row, int column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_row = std::max(row - nr_win_line + 1, 0); // row of first member in first group
    int next_first_row;
    cell_state cur_state = game_data[row][column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    while (group_first_row + nr_win_line - 1 < nr_rows) { // we check that we are still in boundries
        jump = false;
        for (int i = 0; i < nr_win_line; i++) {
            if (game_data[group_first_row + i][column] != cur_state)
            {
                next_first_row = group_first_row + i + 1; // we already know the column that doesn t match
                jump = true;
            }
        }
    
        if (jump == false) { // win line
            // we save row and column for win line endpoints
            win_line_data.start_cell.row = group_first_row;
            win_line_data.start_cell.column = column;    
            win_line_data.stop_cell.row = group_first_row + nr_win_line - 1;
            win_line_data.stop_cell.column = column;
            return true;
        } else {
            group_first_row = next_first_row;
        }
    }

    return false;
}

bool GameLogic::check_win_diag1(int row, int column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = row - std::max(row - nr_win_line + 1, 0);
    int max_col_offset = column - std::max(column - nr_win_line + 1, 0);
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row, group_first_col;
    int next_first_row, next_first_col;
    cell_state cur_state = game_data[row][column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    group_first_row = row - offset;
    group_first_col = column - offset;

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
            win_line_data.start_cell.row = group_first_row;
            win_line_data.start_cell.column = group_first_col;    
            win_line_data.stop_cell.row = group_first_row + nr_win_line - 1;
            win_line_data.stop_cell.column = group_first_col + nr_win_line - 1;
            return true;
        } else {
            group_first_row = next_first_row;
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win_diag2(int row, int column) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = row - std::max(row - nr_win_line + 1, 0);
    // we start from top right corner so we adjust the computations
    int max_col_offset = std::min(column + nr_win_line - 1, nr_columns) - column;
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row, group_first_col;
    int next_first_row, next_first_col;
    cell_state cur_state = game_data[row][column];
    bool jump;

    if (cur_state == CELL_EMPTY) {
        return false;
    }

    group_first_row = row - offset;
    group_first_col = column + offset;

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
            win_line_data.start_cell.row = group_first_row;
            win_line_data.start_cell.column = group_first_col;    
            win_line_data.stop_cell.row = group_first_row + nr_win_line - 1;
            win_line_data.stop_cell.column = group_first_col - nr_win_line + 1;
            return true;
        } else {
            group_first_row = next_first_row;
            group_first_col = next_first_col;
        }
    }

    return false;
}

bool GameLogic::check_win() {
    // DEBUG
    std::cout << "_____\nWIN COND DEBUG:\n";
    std::cout << "ROW WIN: " << check_win_row(cur_pos.row, cur_pos.column)
            << "| COL WIN: " << check_win_column(cur_pos.row, cur_pos.column) 
            << "| DIAG1_WIN: " << check_win_diag1(cur_pos.row, cur_pos.column)
            << "| DIAG2_WIN: " << check_win_diag2(cur_pos.row, cur_pos.column) << "\n";
    //

    // check all possibilities of win
    return check_win_row(cur_pos.row, cur_pos.column) 
        || check_win_column(cur_pos.row, cur_pos.column)
        || check_win_diag1(cur_pos.row, cur_pos.column)
        || check_win_diag2(cur_pos.row, cur_pos.column);
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

player_type Player::get_type() {
    return type;
}

Human::Human(cell_state s, GameLogic* gl, GameGrid* gg)
    : Player(HUMAN, s, gl, gg) {};

bool Human::human_action() {
    cell_pos pos;

    if (game_grid_p->check_mouse_cell(pos) == true) {
        game_grid_p->set_cell_state(pos, used_symbol);
        game_logic_p->set_cell_state(pos, used_symbol);

        return true;
    }
    return false;
}

bool Human::do_next_action() {
    return human_action();
}

Robot::Robot(cell_state s, GameLogic* gl, GameGrid* gg, robot_difficulty diff)
    : Player(ROBOT, s, gl, gg), difficulty(diff) {};

Robot::~Robot() {

};

bool Robot::easy_robot_action() {
    std::vector<cell_pos> available_cells;
    int nr_rows = game_logic_p->get_nr_rows();
    int nr_columns = game_logic_p->get_nr_columns();
    int rand_poz;

    for (int i = 0; i < nr_rows; i++) {
        for (int j = 0; j < nr_columns; j++) {
            if (game_logic_p->get_cell_state({i, j}) == CELL_EMPTY) {
                available_cells.push_back({i, j});
            }
        }
    }

    if (available_cells.size() == 0) {
        return false;
    }

    rand_poz = std::rand() % available_cells.size();

    game_logic_p->set_cell_state({available_cells[rand_poz].row, 
        available_cells[rand_poz].column}, 
        used_symbol
    );

    game_grid_p->set_cell_state({available_cells[rand_poz].row, 
        available_cells[rand_poz].column}, 
        used_symbol
    );

    return true;    
}

bool Robot::hard_robot_action() {
    return false;
}


bool Robot::do_next_action() {
    switch(difficulty) {
        case EASY: return easy_robot_action();
        case HARD: return hard_robot_action();
        default: return false;
    }

    return false;
}

void GameManager::add_player(player_type type, cell_state symbol, robot_difficulty diff) {
    // make sure a player is assigned a proper symbol
    if (symbol == CELL_EMPTY) {
        std::cerr << "Invalid symbol assignation for player's cells state" << "\n";
        return;
    }

    switch (type) {
        case HUMAN: players.push_back(new Human(symbol, game_logic, game_grid)); break;
        case ROBOT: players.push_back(new Robot(symbol, game_logic, game_grid, diff)); break;
        default: break;
    }

    nr_players += 1;
}

GameManager::GameManager() {
    game_window = new GameWindow();

    game_logic = new GameLogic(game_modifiers.nr_rows,
        game_modifiers.nr_columns,
        game_modifiers.nr_win_line
    );

    game_grid = new GameGrid(game_window->get_renderer(),
        game_modifiers.nr_rows,
        game_modifiers.nr_columns,
        game_modifiers.grid_color,
        game_modifiers.color_X,
        game_modifiers.color_0,
        game_modifiers.color_win
    );

    nr_players = 0;
    cur_player = 0;

    add_player(game_modifiers.type1, game_modifiers.symbol1, game_modifiers.diff1);
    add_player(game_modifiers.type2, game_modifiers.symbol2, game_modifiers.diff2);

    srand(time(NULL)); // seed the random generator for potential robot players
}

GameManager::~GameManager() {
    delete game_logic; // destructor should be automatically called
    delete game_grid;
    delete game_window;

    for (Player* player : players) {
        delete player;
    }
}

void GameManager::change_player_turn() {
    cur_player = (cur_player + 1) % nr_players;
}

bool GameManager::decide_win_or_draw() {
    if (game_logic->check_win() == true) {
        game_grid->set_winner(game_logic->get_win_line_data());
        return true;
        
        // DEBUG
        std::cout << "_____\nWIN LINE DRAW DEBUG:\n";
        grid_line_data aux = game_logic->get_win_line_data();
        std::cout << "(" << aux.start_cell.row << "," << aux.start_cell.column << ") -> ";
        std::cout << "(" << aux.stop_cell.row << "," << aux.stop_cell.column << ")" << "\n";
    }

    // check if all cells have been used
    if (game_logic->get_nr_used_cells() == 
        game_logic->get_nr_rows() * game_logic->get_nr_columns()) {
        
        return true;
    }

    return false;
}

void GameManager::DEBUG_func() {
    game_grid->DEBUG_func();
    game_logic->DEBUG_func();
    std::cout << "_____\nPLAYER ORDER DEBUG:\n";
    std:: cout << "NEXT PLAYER ORDER MOVE: " << cur_player <<  "\n";
}

void GameManager::game_loop() {
    bool run_game = true;
    int mouseX, mouseY;

    while(run_game) { 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run_game = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                mouseX = event.button.x;
                mouseY = event.button.y;

                // we set mouse position for a potential action
                game_grid->set_mouse_poz({mouseX, mouseY});
            
                if (players[cur_player]->get_type() == HUMAN) {
                    // make next action and check if it was a succes so we can change turn
                    if (players[cur_player]->do_next_action() == true) {
                        // DEBUG
                        std::cout << "SUCCES HUMAN ACTON WITH NR: " << cur_player << "\n";
                        change_player_turn();
                    } else {
                        // DEBUG
                        std::cout << "FAILURE HUMAN ACTION WITH NR: " << cur_player << "\n";
                    }

                    DEBUG_func();

                    if (decide_win_or_draw() == true) {
                        run_game = false;
                    }

                    // DEBUG
                    std::cout << "\n" << std::endl;
                }
            }
        }

        if (run_game == true) { // make sure game was not already won by previous human action
            if (players[cur_player]->get_type() == ROBOT) {
                if (players[cur_player]->do_next_action() == true) {
                    // DEBUG
                    std::cout << "SUCCES ROBOT ACTON WITH NR: " << cur_player << "\n";
                    change_player_turn();
                } else {
                    // DEBUG
                    std::cout << "FAILURE ROBOT ACTON WITH NR: " << cur_player << "\n";
                }

                DEBUG_func();

                if (decide_win_or_draw() == true) {
                    run_game = false;
                }

                // DEBUG
                std::cout << "\n" << std::endl;
            }
        }

        game_window->prepare_render();
        //game_grid->draw_grid();
        game_window->render();

        run_game == false ? SDL_Delay(game_modifiers.big_delay) : 
            SDL_Delay(game_modifiers.small_delay);
    }
}