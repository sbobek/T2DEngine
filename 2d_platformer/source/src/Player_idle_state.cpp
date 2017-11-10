#include "Gameplay_state.hpp"
#include "Player_idle_state.hpp"
#include "Player_running_state.hpp"
#include "Player_jumping_state.hpp"
#include "Actor.hpp"
#include "Input_manager.hpp"
#include "Button.hpp"
#include "Body_2d.hpp"
#include "AABB_2d.hpp"
#include "Timer.hpp"
#include <iostream>
#include <algorithm>
#include "game.hpp"


//TODO: SWITCH ON ACTION BUTTONS, I.E. ATTACK, RUN_RIGHT, ETC
Gameplay_state * Player_idle_state::check_transition(Actor & actor)
{
	//auto stream = Input_handler::instance().get_input();

	bool on_ground = g_physics_manager.get_world()->is_body_2d_on_ground(actor.get_body_2d());

	//auto iter = std::find_if(stream.begin(), stream.end(),
	//	[](const std::pair<Button, Command*> & p) {return p.first.m_bound_key == KEY_A; });
	
	const Button & move_left_button = g_input_manager.get_button_from_action(Input_manager::GAME_ACTIONS::MOVE_LEFT);

	if (move_left_button.m_state == PRESSED) {
		std::cout << "chaging state to Player_running_state| dir= left" << std::endl;
		actor.set_facing_direction(true);     //change to running left
		 //set the parameter on the  animation state machine
		actor.get_sprite().get_panim_controller()->set_bool("is_running", true);
		return new Player_running_state(actor);
	}
	
	const Button & move_right_button = g_input_manager.get_button_from_action(Input_manager::GAME_ACTIONS::MOVE_RIGHT);
	
	if (move_right_button.m_state == PRESSED) {
		std::cout << "Changing state to Player_running_state | dir = right" << std::endl;
			
		//set the parameter on the animation state machine
		actor.get_sprite().get_panim_controller()->set_bool("is_running", true);

		actor.set_facing_direction(false);///change to running right
		return new Player_running_state(actor);
	}
	if (on_ground) {
		const Button & jump_button = g_input_manager.get_button_from_action(Input_manager::GAME_ACTIONS::JUMP);
		if ( jump_button.m_state == PRESSED ) {
			std::cout << "Changing to Player_jumping_state" << std::endl;
			actor.get_sprite().get_panim_controller()->set_bool("is_jumping", true);
			return new Player_jumping_state(actor);
		}
	}
	return nullptr;
}

void Player_idle_state::begin_tile_collision(Actor & actor, const AABB_2d & tile_aabb) 
{
	std::cout << "PLAYER IDLE STATE " << __FUNCTION__ << ": with ";

	physics_2d::Body_2d  *pbody = actor.get_body_2d();

	if (pbody->get_aabb_2d().p_min.y >= tile_aabb.p_max.y && (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x) ) {
		std::cout << "Floor tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_max.y <= tile_aabb.p_min.y && (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x)) {
		std::cout << "Ceiling tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && (pbody->get_aabb_2d().p_min.y < tile_aabb.p_max.y && pbody->get_aabb_2d().p_max.y > tile_aabb.p_min.y)) {
		std::cout << "Right wall tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x && (pbody->get_aabb_2d().p_min.y < tile_aabb.p_max.y && pbody->get_aabb_2d().p_max.y > tile_aabb.p_min.y)) {
		std::cout << "Left wall tile " << std::endl;
	}
	else {
		std::cerr << " unknow tile aabb coordinate..." << std::endl;
	}
}

void Player_idle_state::end_tile_collision(Actor & actor, const AABB_2d & tile_aabb) 
{
	std::cout << "PLAYER IDLE STATE " << __FUNCTION__ << ": with ";

	physics_2d::Body_2d  *pbody = actor.get_body_2d();

	if (pbody->get_aabb_2d().p_min.y >= tile_aabb.p_max.y && (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x)) {
		std::cout << "Floor tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_max.y <= tile_aabb.p_min.y && (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x)) {
		std::cout << "Ceiling tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_max.x >= tile_aabb.p_min.x && (pbody->get_aabb_2d().p_min.y < tile_aabb.p_max.y && pbody->get_aabb_2d().p_max.y > tile_aabb.p_min.y)) {
		std::cout << "Right wall tile" << std::endl;
	}
	else if (pbody->get_aabb_2d().p_min.x <= tile_aabb.p_max.x && (pbody->get_aabb_2d().p_min.y < tile_aabb.p_max.y && pbody->get_aabb_2d().p_max.y > tile_aabb.p_min.y)) {
		std::cout << "Left wall tile " << std::endl;
	}
	else {
		std::cerr << " unknow tile aabb coordinate..." << std::endl;
	}
}

void Player_idle_state::update(Actor & actor) 
{
	//if (!get_anim_player().is_playing()) {
	//	if (m_blink_timer < Timer::instance().get_time()) {
	//		get_anim_player().resume();
	//	}
	//}

	//the only behavior of idle is playing the idle animation
//	unsigned curr_frame = get_anim_player().get_frame();
//	get_anim_player().update();
//	if (curr_frame != get_anim_player().get_frame()) {
		///change texture coordinates
		//PUT THIS ON A DIFFERENT FUCTION, I.E, NOT PLAYER'S UPDATE FUNCTION, IT IS A RENDERING FUCTNIO!!!!!
	//	actor.get_sprite().update_uv(get_anim_player().get_frame());
	//	if (get_anim_player().get_frame() == 1) { //blinked
	//		m_blinked = true;
	//	}
//		else if ((get_anim_player().get_frame() == 0) && ( m_blinked) ) {
//			get_anim_player().pause();
//			m_blink_timer = Timer::instance().get_time() + BLINK_INTERVAL;
//			m_blinked = false;
//		}
//	}
	actor.get_sprite().update_pos(actor.get_body_2d()->get_position(), actor.get_facing_direction());
	return;
}