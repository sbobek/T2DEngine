#ifndef _CREATOR_HPP
#define _CREATOR_HPP

#include <cstddef>
#include <stdint.h>

/* Creator: this class is used with the Game_object_factory class, to be able to dynamically create objects at runtime 
 * Every class that wants to register itself with the factory, should have a correspoding Creator class, i.e
 * the Zombie class should have a Creator_zombie that inherits from the Creator
 */
namespace math { struct vec3; }
namespace gom  { class Game_object; }
namespace physics_2d {struct Body_2d_def; }

namespace gom {
	class Creator {
	public:
		Creator() : m_pbody_def(nullptr) {}
		virtual ~Creator();

		virtual Game_object *create(const math::vec3 & wld_pos) = 0;
		
		void		 set_obj_type(const uint32_t type) { m_obj_type = type; }
		uint32_t     get_obj_type() const { return m_obj_type; }
        void         set_obj_tag(const uint32_t tag) { m_obj_tag = tag; }
        uint32_t     get_obj_tag() const { return m_obj_tag; }
	protected:
		physics_2d::Body_2d_def *m_pbody_def; // used to create Body_2d for game objects that need physics simulation
		uint32_t				 m_obj_type;
        uint32_t                 m_obj_tag;
	};
}
#endif // !_CREATOR_HPP
