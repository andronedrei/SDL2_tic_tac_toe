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

Robot::Robot(cell_state s, GameLogic* gl, GameGrid* gg, robot_difficulty diff)
    : Player(ROBOT, s, gl, gg), difficulty(diff) {};

Robot::~Robot() {};

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