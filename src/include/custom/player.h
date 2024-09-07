#ifndef PLAYER_H
#define PLAYER_H

#include "custom/utils.h"
#include "custom/game_logic.h"

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

#endif