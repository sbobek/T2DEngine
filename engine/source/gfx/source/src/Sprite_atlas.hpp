#ifndef _SPRITE_ATLAS_HPP
#define _SPRITE_ATLAS_HPP

#include "Resource.hpp"
#include "Sprite_atlas_manager.hpp"
#include "Rect.hpp"
#include "string_id.hpp"
#include <vector>

/*Sprite_atlas: this class stores all the data associated with
 * a sprite atlas necessary for rendering it, including the texture_2d  
 * object and, a vector of rect objects containg the UV coordinates for
 * all the sprites in the atlas
 */
namespace rms { class Resource_manager; }
namespace gfx { class Texture_2d; class Texture_2d_manager; }

namespace gfx {
	class Sprite_atlas : public rms::Resource {
	friend rms::Resource *gfx::Sprite_atlas_manager::load(const char *name, const char *file_path, gfx::Texture_2d_manager *texture_manager);
	public:
		Sprite_atlas(rms::Resource_manager *pcreator, const char *name);

		void load()    override;
		void unload()  override;
		size_t calculate_size() const override;

		void set_atlas_file_path(const char *file_path);
		const char *get_texture_file_path() const;

		//Sprite_atlas(const std::string & file_name, const string_id id);
		
		//Texture_2d &      get_texture()		    { return m_texture; }
		
		//string_id			get_string_id()		   const { return m_id; }
		gfx::Texture_2d *get_texture();
		void get_text_coord(const int sprite_no, math::Rect * prect, float * px_width, float *px_height) const;
		
		//~Sprite_atlas();
	private:
		//string_id				m_id;					// size 4 bytes | alignment 4 bytes
		
		gfx::Texture_2d   *m_ptexture;				// size 16 bytes | alignment 4 bytes
	    char			  *m_atlas_file_path;
		char			  *m_texture_file_path;

	// pixel coordinates of each sprite on the texture atlas
		std::vector<math::Rect>	m_vrec;					// 12 bytes size 4 byte aligment 

	  //TODO: SCALE THE VERTICES IN THE POSITION VECTOR WHEN CHANGING THE CURRENT VERTEX UV COORDINATES			
	};
}

#endif // !_SPRITE_ATLAS_HPP
