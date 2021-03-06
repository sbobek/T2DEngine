#ifndef _PLAYER_TAKING_HIT_STATE_HPP
#define _PLAYER_TAKING_HIT_STATE_HPP

#include "Gameplay_state.hpp"
#include "AABB_2d.hpp"

namespace gom { class Actor; }
namespace physics_2d { struct AABB_2d; class World; }

class Player_taking_hit_state : public gom::Gameplay_state {
public:
        Player_taking_hit_state(gom::Actor & actor, float jump_vel = 10.0f, float x_vel = 2.5f);

        gom::Gameplay_state *  handle_input(gom::Actor & actor) override;
        size_t get_size() const override;

        virtual void begin_tile_collision(gom::Actor & actor, const physics_2d::AABB_2d & tile_aabb) override;
        virtual void end_tile_collision(gom::Actor & actor, const physics_2d::AABB_2d & tile_aabb) override;
};

#endif // !_PLAYER_TAKING_HIT_STATE_HPP
