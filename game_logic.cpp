#include <vector>
#include <iostream>

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

    cur_row = 0;
    cur_column = 0;

    win_line_data.start_row = 0;
    win_line_data.start_column = 0;
    win_line_data.stop_row = 0;
    win_line_data.stop_column = 0;
}

GameLogic::~GameLogic() {};

void GameLogic::set_cell_state(int row, int column, cell_state state) {
    game_data[row][column] = state;

    cur_row = row;
    cur_column = column;
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
            win_line_data.start_row = row;
            win_line_data.start_column = group_first_col;
            win_line_data.stop_row = row;
            win_line_data.stop_column = group_first_col + nr_win_line - 1;
       
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
            win_line_data.start_row = group_first_row;
            win_line_data.start_column = column;    
            win_line_data.stop_row = group_first_row + nr_win_line - 1;
            win_line_data.stop_column = column;
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

bool GameLogic::check_win() {
    ////
    // std::cout << "ROW: " << check_win_row(cur_row, cur_column)
    //         << "| COL: " << check_win_column(cur_row, cur_column) 
    //         << "| DIAG1: " << check_win_diag1(cur_row, cur_column)
    //         << "| DIAG2: " << check_win_diag2(cur_row, cur_column) << "\n";
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

player_type Player::get_type() {
    return type;
}

Human::Human(cell_state s, GameLogic* gl, GameGrid* gg)
    : Player(HUMAN, s, gl, gg) {};

bool Human::human_action(SDL_Point mouse_poz) {
    int row, column;

    if (game_grid_p->check_mouse_cell(mouse_poz, row, column) == true) {
        game_grid_p->set_cell_state(row, column, used_symbol);
        game_logic_p->set_cell_state(row, column, used_symbol);

        return true;
    }
    return false;
}

bool Human::do_next_action(SDL_Point mouse_poz) {
    return human_action(mouse_poz);
}

Robot::Robot(cell_state s, GameLogic* gl, GameGrid* gg)
    : Player(ROBOT, s, gl, gg) {};

Robot::~Robot() {

};

bool Robot::robot_action() {
    return false;
}

bool Robot::do_next_action(SDL_Point mouse_poz) {
    return robot_action();
}

void GameManager::add_player(player_type type, cell_state symbol) {
    switch (type) {
        case HUMAN: players.push_back(new Human(symbol, game_logic, game_grid)); break;
        case ROBOT: players.push_back(new Robot(symbol, game_logic, game_grid)); break;
        default: break;
    }

    nr_players += 1;
}

GameManager::GameManager() {
    GameModifiers game_modifiers; // modify source later

    // initialize SDL related aspects
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("TicTacToe", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        720,
        480,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError();
    }

    renderer = SDL_CreateRenderer(window,
        -1, 
        SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        std::cerr << "Could not create renderer: " << SDL_GetError();
    }

    game_logic = new GameLogic(game_modifiers.nr_rows,
        game_modifiers.nr_columns,
        game_modifiers.nr_win_line
    );

    game_grid = new GameGrid(renderer,
        game_modifiers.nr_rows,
        game_modifiers.nr_columns,
        game_modifiers.grid_color,
        game_modifiers.color_X,
        game_modifiers.color_0,
        game_modifiers.color_win
    );

    nr_players = 0;
    cur_player = 0;

    add_player(game_modifiers.type1, game_modifiers.symbol1);
    add_player(game_modifiers.type2, game_modifiers.symbol2);
}

GameManager::~GameManager() {
    delete game_logic; // destructor should be automatically called
    delete game_grid;

    for (Player* player : players) {
        delete player;
    }
}

void GameManager::change_player_turn() {
    cur_player = (cur_player + 1) % nr_players;
}

bool GameManager::decide_win() {
    if (game_logic->check_win() == true) {
        game_grid->set_winner(game_logic->get_win_line_data());
        return true;
    }

    return false;
}

void GameManager::game_loop() {
    bool run_game = true;
    bool delay_more = false;
    int mouseX, mouseY;

    while(run_game) { 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run_game = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                mouseX = event.button.x;
                mouseY = event.button.y;
            
                if (players[cur_player]->get_type() == HUMAN) {
                    if (players[cur_player]->do_next_action({mouseX, mouseY}) == true) {
                        change_player_turn();
                    }
                }
            }
        }

        if (players[cur_player]->get_type() == ROBOT) {
            if (players[cur_player]->do_next_action({mouseX, mouseY}) == true) {
                change_player_turn();
            }
        }

        if (decide_win() == true) {
            run_game = false;
            delay_more = true;
        }

        // clear renderer before drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black background
        SDL_RenderClear(renderer);

        // main logic of game
        game_grid->draw_grid();

        // update renderer to display image
        SDL_RenderPresent(renderer);

        if (delay_more == true) {
            SDL_Delay(2000);
        } else {
            SDL_Delay(25);
        }
    }
}

void GameManager::exit_game_window() {
    SDL_DestroyRenderer(renderer);    
    SDL_DestroyWindow(window);
    SDL_Quit();
}