#include <algorithm>
#include "Player_jumping_state.hpp"
#include "Player_idle_state.hpp"
#include "Actor.hpp"
#include "Gameplay_state.hpp"
#include "vec2.hpp"
#include "AABB_2d.hpp"
#include "Body_2d.hpp"
#include "Input_manager.hpp"
#include "game.hpp"

Player_jumping_state::Player_jumping_state(Actor & actor, float y_acceleration, float x_vel) 
	: Gameplay_state(), m_y_acceleration(y_acceleration), m_x_vel(x_vel), m_moving_left(false), m_moving_right(false)
{
	actor.get_body_2d()->add_force(cgm::vec2(0.0f, m_y_acceleration));
}

Gameplay_state * Player_jumping_state::check_transition(Actor & actor) 
{
	//no air controll, for now...

	bool on_ground = g_physics_manager.get_world()->is_body_2d_on_ground(actor.get_body_2d());

	if (on_ground) {
		//std::cout << "changing state to player_idle" << std::endl;
		//set the paramter on the animation state machine to make the transition to the new animation
		actor.get_sprite().get_panim_controller()->set_bool("is_jumping", false);
		actor.get_body_2d()->stop_movement_x();
		return new Player_idle_state;
	}

	const Button & move_left_button = g_input_manager.get_button_from_action(Input_manager::GAME_ACTIONS::MOVE_LEFT);
	if (move_left_button.m_state == PRESSED && !m_moving_left) {
		actor.set_facing_direction(true);     //change to running left
		actor.get_body_2d()->add_force(cgm::vec2(-m_x_vel, 0.0f));
		//std::cout << "-----adding force here---------" << std::endl;
		m_moving_left = true;
	}
	else if (move_left_button.m_state == RELEASED) {
		m_moving_left = false;
	}


	const Button & move_right_button = g_input_manager.get_button_from_action(Input_manager::GAME_ACTIONS::MOVE_RIGHT);
	if (move_right_button.m_state == PRESSED && !m_moving_right) {
		actor.set_facing_direction(false);     
		actor.get_body_2d()->add_force(cgm::vec2(m_x_vel, 0.0f));
		m_moving_right = true;
	}
	else if (move_right_button.m_state == RELEASED) {
		//actor.get_body_2d()->stop_movement_x();
		m_moving_right = false;
	}
	
	if ( !m_moving_left && !m_moving_right) {
		//actor.get_body_2d()->stop_movement_x();
	}
	return nullptr;
}

void Player_jumping_state::update(Actor & actor) 
{
	actor.get_sprite().update_pos(actor.get_body_2d()->get_position(), actor.get_facing_direction());
}

void Player_jumping_state::begin_tile_collision(Actor & actor, const AABB_2d & tile_aabb) 
{
	std::cout << __FUNCTION__ << std::endl;
}

void Player_jumping_state::end_tile_collision(Actor & actor, const AABB_2d & tile_aabb)
{
	std::cout << __FUNCTION__ << std::endl;
}