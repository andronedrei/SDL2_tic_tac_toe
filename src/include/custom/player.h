#ifndef PLAYER_H
#define PLAYER_H

#include <bits/stdc++.h>

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
    std::vector<cell_state>& symbols_order; // reference to symbols order
  
    // helper variables for different robot functions
    std::vector<cell_pos> available_cells;
    std::vector<bool> marked_cells;
    std::stack<cell_pos> moves_record;
    int cur_player;
    int nr_players;

    // helper functions used for higher difficulties robots
  
    void next_player_turn();
    void last_player_turn();
    // check if current game state is terminal
    bool is_terminal(bool& win_termination);
    // returns a "score" based on favorability for the curent player (in a terminal state)
    int evaluate_game_state(bool& win_termination);
    void simulate_player_action(cell_pos pos);
    void revert_action_simulation();
    cell_pos minimax();
    // helper function that calls itself recursively
    int minimax_helper(int cur_depth, int max_depth, cell_pos& optimal_pos);

    void robot_round_setup();
    bool easy_robot_action();
    bool hard_robot_action();

  public:
    Robot(cell_state s, GameLogic* gl, GameGrid* gg, 
      robot_difficulty diff, std::vector<cell_state>& symb_order); 
    ~Robot() override;

    bool do_next_action() override;
};

#endif