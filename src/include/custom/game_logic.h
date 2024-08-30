#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <vector>

#include "custom/utils.h"

class GameGrid;

class GameLogic {
  private:
    std::vector<std::vector<cell_state>> game_data;
    int nr_rows;
    int nr_columns;
    int nr_win_line; // nr of cells neccesary for winning the game
    int cur_row; // curent row where a cell was modified
    int cur_column;  // curent column where a cell was modified
    grid_line_data win_line_data;

    bool check_win_row(int row, int column);
    bool check_win_column(int row, int column);
    bool check_win_diag1(int row, int column);
    bool check_win_diag2(int row, int column);
  public:
    void set_cell_state(int row, int column, cell_state state);
    // check if last move conducted to a win 
    bool check_win();
    grid_line_data get_win_line_data();

    GameLogic(int n_rows, int n_cols, int n_win_line);
    ~GameLogic();
    void clear_game_data();
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

    virtual bool do_next_action(SDL_Point mouse_poz) = 0;
};

class Human : public Player {
  private:
    bool human_action(SDL_Point mouse_poz);

  public:
    Human(cell_state s, GameLogic* gl, GameGrid* gg);

    bool do_next_action(SDL_Point mouse_poz) override;
};

class Robot : public Player  {
  private:
    bool robot_action();

  public:
    Robot(cell_state s, GameLogic* gl, GameGrid* gg);
    ~Robot() override;

    bool do_next_action(SDL_Point mouse_poz) override;
};

class GameManager {
  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    
    GameLogic* game_logic;
    GameGrid* game_grid;
    std::vector<Player*> players; // nr of players should be 2 or 3

    int nr_players;
    int cur_player;

    void add_player(player_type type, cell_state symbol);
    void change_player_turn();
    bool decide_win(); //function to decide win and make necessary changes

  public:
    GameManager();
    ~GameManager();
    void game_loop();
    void exit_game_window();
};

#endif