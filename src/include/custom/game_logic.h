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
    grid_line_data win_line_data;

    bool check_win_row(int cur_row, int cur_column);
    bool check_win_column(int cur_row, int cur_column);
    bool check_win_diag1(int cur_row, int cur_column);
    bool check_win_diag2(int cur_row, int cur_column);
  public:
    void set_cell_state(int row, int column, cell_state state);
    // check if last move conducted to a win 
    bool check_win(int cur_row, int cur_column);
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

    virtual void do_next_action() = 0;
};

class Human : public Player {
  private:
    void human_action();

  public:
    Human(cell_state s, GameLogic* gl, GameGrid* gg);

    void do_next_action() override;
};

class Robot : public Player  {
  private:
    void robot_action();

  public:
    Robot(cell_state s, GameLogic* gl, GameGrid* gg);
    ~Robot() override;

    void do_next_action() override;
};

#endif