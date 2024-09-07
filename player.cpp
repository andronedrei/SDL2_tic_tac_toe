#include "custom/utils.h"
#include "custom/player.h"
#include "custom/game_logic.h"
#include "custom/game_interface.h"

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

Robot::Robot(cell_state s, GameLogic* gl, GameGrid* gg, 
    robot_difficulty diff, std::vector<cell_state>& symb_order)
    : Player(ROBOT, s, gl, gg), difficulty(diff), symbols_order(symb_order) {};

Robot::~Robot() {};

void Robot::robot_round_setup() {
    // prepare used data structures (old one are probably destroyed automatically)
    available_cells = game_logic_p->get_available_cells();
    marked_cells = std::vector(available_cells.size(), false);
    moves_record = std::stack<cell_pos>();

    // prepare player order for games simulations by robot
    for (int i = 0; i < symbols_order.size(); i++) {
        if (symbols_order[i] == used_symbol) {
            cur_player = i;
            break;
        }
    }
    nr_players = symbols_order.size();
}

bool Robot::easy_robot_action() {
    int rand_poz;

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
    cell_pos action_pos = minimax();

    if (available_cells.size() == 0) {
        return false;
    }

    game_logic_p->set_cell_state(action_pos, used_symbol);
    game_grid_p->set_cell_state(action_pos, used_symbol);

    return true;
}

bool Robot::do_next_action() {
    // setup for different kinds of robot actions
    robot_round_setup();

    switch(difficulty) {
        case EASY: return easy_robot_action();
        case HARD: return hard_robot_action();
        default: return false;
    }

    return false;
}

// helper functions used for higher difficulties robots

void Robot::next_player_turn() {
    cur_player = (cur_player + 1) % symbols_order.size();
}

void Robot::last_player_turn() {
    cur_player = (cur_player - 1 + symbols_order.size()) % symbols_order.size();
}

bool Robot::is_terminal(bool& win_termination) {
    // check if last player won the game
    if (game_logic_p->check_win()) {
        win_termination = true;
        return true;
    }

    // check if all possible moves where made
    if (available_cells.size() == moves_record.size()) {
        win_termination == false;
        return true;
    }

    return false;
}

// call if game is terminal to get score
int Robot::evaluate_game_state(bool& win_termination) {
    if (win_termination == false) {
        return 0; // draw game
    }

    // check if cur player was one who made last move (and won essentialy)
    if (game_logic_p->get_cell_state(moves_record.top()) == used_symbol) {
        return 1;
    }
    return -1; // one of opponents won
}

void Robot::simulate_player_action(cell_pos pos) {
    game_logic_p->set_cell_state(pos, symbols_order[cur_player]);
    next_player_turn();

    moves_record.push(pos);
}

void Robot::revert_action_simulation() {
    game_logic_p->set_cell_state(moves_record.top(), CELL_EMPTY);
    last_player_turn();

    moves_record.pop();
}

cell_pos Robot::minimax() {
    cell_pos optimal_pos = available_cells[0];
    int val = minimax_helper(0, 100, optimal_pos);

    return optimal_pos;
}

int Robot::minimax_helper(int cur_depth, int max_depth, cell_pos& optimal_pos) {
    bool win_termination;
    if (is_terminal(win_termination) == true) {
        return evaluate_game_state(win_termination);
    }

    int sz = available_cells.size();
    if (symbols_order[cur_player] == used_symbol) {
        // cur player turn (we maximize)
        int max_val = INT_MIN;
        int old_max;
    
        for (int index = 0; index < sz; index++) {
            if (marked_cells[index] == false) { // a move can be explored
                // simulate action with cur pos
                marked_cells[index] = true;
                simulate_player_action(available_cells[index]);

                old_max = max_val;
                max_val = std::max(max_val, minimax_helper(cur_depth + 1, max_depth, optimal_pos));
            
                // check if we found a new more optimal solution for actual cur player move with simulations
                if (cur_depth == 0 && old_max != max_val) {
                    optimal_pos = available_cells[index];
                }

                // revert action
                marked_cells[index] = false;
                revert_action_simulation();
            }
        }

        return max_val;
    } else {
        // other player turn (we asume he minimize)
        int min_val = INT_MAX;
    
        for (int index = 0; index < sz; index++) {
            if (marked_cells[index] == false) { // a move can be explored
                // simulate action with cur pos
                marked_cells[index] = true;
                simulate_player_action(available_cells[index]);

                min_val = std::min(min_val, minimax_helper(cur_depth + 1, max_depth, optimal_pos));

                // revert action
                marked_cells[index] = false;
                revert_action_simulation();
            }
        }

        return min_val;
    }

    return 0;
}




