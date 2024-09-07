#include <vector>
#include <iostream>
#include <cstdlib>

#include "custom/game_logic.h"
#include "custom/game_interface.h"
#include "custom/utils.h"
#include "custom/player.h"

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

bool GameLogic::check_group(cell_state target_state, cell_pos& start, int row_dir, int col_dir) {
    cell_state cur_cell_state;
    int cur_row;
    int cur_col;

    for (int i = 0; i < nr_win_line; i++) {
        cur_row = start.row + i * row_dir;
        cur_col = start.column + i * col_dir;
        cur_cell_state = game_data[cur_row][cur_col];
        if (cur_cell_state != target_state) {
            // we should start from next cell now
            start.row = cur_row + row_dir;
            start.column = cur_col + col_dir;
            return false;
        }
    }

    return true;
}

bool GameLogic::check_win_row(cell_pos pos) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_col = std::max(pos.column - nr_win_line + 1, 0); // col of first member in first group
    cell_pos group_first = {pos.row, group_first_col};
    cell_state target_state = game_data[pos.row][pos.column];

    if (target_state == CELL_EMPTY) {
        return false;
    }

    // check bounds and go trough each group
    while (group_first.column + nr_win_line - 1 < nr_columns) {
        if (check_group(target_state, group_first, 0, 1) == true) {
            win_line_data = {group_first, // start cell for win line
                {group_first.row, group_first.column + nr_win_line - 1} // stop cell for win line
            };
            return true;
        }
    }

    return false;
}

bool GameLogic::check_win_column(cell_pos pos) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int group_first_row = std::max(pos.row - nr_win_line + 1, 0); // row of first member in first group
    cell_pos group_first = {group_first_row, pos.column};
    cell_state target_state = game_data[pos.row][pos.column];

    if (target_state == CELL_EMPTY) {
        return false;
    }

    // check bounds and go trough each group
    while (group_first.row + nr_win_line - 1 < nr_rows) {
        if (check_group(target_state, group_first, 1, 0) == true) {
            win_line_data = {group_first, // start cell for win line
                {group_first.row + nr_win_line - 1, group_first.column} // stop cell for win line
            };
            return true;
        }
    }

    return false;
}

bool GameLogic::check_win_diag1(cell_pos pos) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = pos.row - std::max(pos.row - nr_win_line + 1, 0);
    int max_col_offset = pos.column - std::max(pos.column - nr_win_line + 1, 0);
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row = pos.row - offset;
    int group_first_col = pos.column - offset;
    cell_pos group_first = {group_first_row, group_first_col};
    cell_state target_state = game_data[pos.row][pos.column];

    // check bounds and go trough each group
    while (group_first.row + nr_win_line - 1 < nr_rows
        && group_first.column + nr_win_line - 1 < nr_columns) {

        if (check_group(target_state, group_first, 1, 1) == true) {
            win_line_data = {group_first, // start cell for win line
                {group_first.row + nr_win_line - 1, group_first.column + nr_win_line - 1}
            };
            return true;
        }
    }

    return false;
}

bool GameLogic::check_win_diag2(cell_pos pos) {
    // we verify all "nr_win_line" sized groups that contain current cell
    // we find first and last possible groups
    int max_row_offset = std::min(pos.row + nr_win_line - 1, nr_rows - 1) - pos.row;
    int max_col_offset = pos.column - std::max(pos.column - nr_win_line + 1, 0);
    int offset = std::min(max_row_offset, max_col_offset);
    
    int group_first_row = pos.row + offset;
    int group_first_col = pos.column - offset;
    cell_pos group_first = {group_first_row, group_first_col};
    cell_state target_state = game_data[pos.row][pos.column];

    // check bounds and go trough each group
    while (group_first.row - nr_win_line + 1 >= 0
        && group_first.column + nr_win_line - 1 < nr_columns) {

        if (check_group(target_state, group_first, -1, 1) == true) {
            win_line_data = {group_first, // start cell for win line
                {group_first.row - nr_win_line + 1, group_first.column + nr_win_line - 1}
            };
            return true;
        }
    }

    return false;
}

bool GameLogic::check_win() {
    // DEBUG
    std::cout << "_____\nWIN COND DEBUG:\n";
    std::cout << "ROW WIN: " << check_win_row(cur_pos)
            << "| COL WIN: " << check_win_column(cur_pos) 
            << "| DIAG1_WIN: " << check_win_diag1(cur_pos)
            << "| DIAG2_WIN: " << check_win_diag2(cur_pos) << "\n";
    

    if (nr_win_line > std::min(nr_columns, nr_rows)) {
        std::cerr << "Invalid game logic, nr cells necesary for win is too big\n"; 
    }

    // check all possibilities of win
    return check_win_row(cur_pos) 
        || check_win_column(cur_pos)
        || check_win_diag1(cur_pos)
        || check_win_diag2(cur_pos);
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
        game_modifiers.color_Z,
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
    if (game_logic->get_nr_used_cells() >= 
        game_logic->get_nr_rows() * game_logic->get_nr_columns()) {
        std::cout << "____\nDRAW\n";
        return true;
    }

    return false;
}

void GameManager::handle_resize_event() {
    game_window->handle_resize();
    game_grid->update_grid_dim();
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
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    handle_resize_event();
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
        game_grid->draw_grid();
        game_window->render();

        run_game == false ? SDL_Delay(game_modifiers.big_delay) : 
            SDL_Delay(game_modifiers.small_delay);
    }
}