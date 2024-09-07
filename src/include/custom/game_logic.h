#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <vector>
#include <SDL2/SDL.h>

#include "custom/utils.h"

class GameWindow;
class GameGrid;

class GameLogic {
  private:
    std::vector<std::vector<cell_state>> game_data;
    int nr_used_cells;
    int nr_rows;
    int nr_columns;
    int nr_win_line; // nr of cells neccesary for winning the game
    cell_pos cur_pos; // curent row and column where a cell was modified
    grid_line_data win_line_data;

    // function to check if a group of nr_win_line contains only one symbol
    // a row_dir of 0 means group is the same row (analog for row dir)
    // a row dir of -1 or 1 means going to next or previous row in grid
    // for going diagonally pick both row_dir and col_dir to be 1 (or -1 for bacwards diag)
    // in case group is a mismatch, next group start cell for check is saved in start
    bool check_group(cell_state target_state, cell_pos& start, int row_dir, int col_dir);
    bool check_win_row(cell_pos pos);
    bool check_win_column(cell_pos pos);
    bool check_win_diag1(cell_pos pos);
    bool check_win_diag2(cell_pos pos);
  public:
    // getters and setters
    cell_state get_cell_state(cell_pos pos);
    int get_nr_rows();
    int get_nr_columns();
    int get_nr_used_cells();
    void set_cell_state(cell_pos pos, cell_state state);

    // check if last move conducted to a win 
    bool check_win();
    grid_line_data get_win_line_data();

    GameLogic(int n_rows, int n_cols, int n_win_line);
    ~GameLogic();
    void clear_game_data();

    void DEBUG_func();
};

// abstract class
class Player {
  protected:
    player_type type;
    cell_state used_symbol;
    GameLogic* game_logic_p;
    GameGrid* game_grid_p;

  public:
    Player(player_type t, cell_state s, GameLogic* gl, GameGrid* gg);
    virtual ~Player();
    player_type get_type();

    virtual bool do_next_action() = 0;
};

class Human : public Player {
  private:
    bool human_action();

  public:
    Human(cell_state s, GameLogic* gl, GameGrid* gg);

    bool do_next_action() override;
};

class Robot : public Player  {
  private:
    robot_difficulty difficulty;
    bool easy_robot_action();
    bool hard_robot_action();

  public:
    Robot(cell_state s, GameLogic* gl, GameGrid* gg, robot_difficulty diff);
    ~Robot() override;

    bool do_next_action() override;
};

class GameManager {
  private:
    GameModifiers game_modifiers; // variables that affect game and can be easily modified
  
    GameWindow* game_window;  
    GameLogic* game_logic;
    GameGrid* game_grid;
    std::vector<Player*> players; // nr of players should be 2 or 3

    SDL_Event event;
    int nr_players;
    int cur_player;

    void add_player(player_type type, cell_state symbol, robot_difficulty diff);
    void change_player_turn();
    bool decide_win_or_draw(); //function to decide ppotential win or draw and make necessary changes
    void handle_resize_event();

    void DEBUG_func();

  public:
    GameManager();
    ~GameManager();
    void game_loop();
};

#endif