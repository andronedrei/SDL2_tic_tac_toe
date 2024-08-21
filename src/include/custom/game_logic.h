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
    int nr_cross_win; // nr of cells neccesary for winning the game

  public:
    GameLogic(int n_rows, int n_cols, int n_cross_win);
    ~GameLogic(); // auto generated
    void clear_game_data();
};

// abstract class
class Player {
  protected:
    player_type type;
    symbol used_symbol;
    GameLogic* game_logic_p;
    GameGrid* game_grid_p;

  public:
    Player(player_type t, symbol s, GameLogic* gl, GameGrid* gg);
    virtual ~Player();

    virtual void do_next_action() = 0;
};

class Human : public Player {
  private:
    void human_action();

  public:
    Human(symbol s, GameLogic* gl, GameGrid* gg);

    void do_next_action() override;
};

class Robot : public Player  {
  private:
    void robot_action();

  public:
    Robot(symbol s, GameLogic* gl, GameGrid* gg);
    ~Robot() override;

    void do_next_action() override;
};

#endif