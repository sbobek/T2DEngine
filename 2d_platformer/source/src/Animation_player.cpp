#include "Animation_player.hpp"
#include "Animation.hpp"
#include "Timer.hpp"
#include <vector>
#include <iostream>

tgs::Animation_player::Animation_player(const std::vector<Animation> & animations) : m_animations(animations), m_current_animation(0), m_playing(true), m_current_frame(-1)
{
	m_seconds_per_frame = 1.0f / m_animations[m_current_animation].frames_per_second();
	m_next_frame_time   = Timer::instance().get_time();
}

void tgs::Animation_player::update() 
{
	if ( (m_next_frame_time > Timer::instance().get_time()) || (!m_playing) ) { //if it is not yet time to change to the next frame 
		return;
	}
	
	++m_current_frame;
	
	if (m_current_frame >= m_animations[m_current_animation].num_frames()) { // played the last frame
		if (m_animations[m_current_animation].loop()) { // looped animation
			m_current_frame = 0;
		}
		else {
			m_playing = false;
			return;
		}
	}
	m_next_frame_time = Timer::instance().get_time() + m_seconds_per_frame; //calculate when to switch to next frame

	std::cout << "current frame: " << m_current_frame << "  next frame time: " << m_next_frame_time << std::endl;
}

//pauses the current animation
void tgs::Animation_player::pause() 
{
	m_playing = false;
}

void tgs::Animation_player::resume() 
{
	if (!m_playing) {
		m_playing = true;
		m_next_frame_time = Timer::instance().get_time() + m_seconds_per_frame;
	}
}