#include "Sprite_atlas.hpp"
#include "Resource.hpp"
#include "Resource_manager.hpp"
#include "Texture_2d.hpp"
#include "string_id.hpp"
#include "Rect.hpp"

#include <string>
#include <iostream>
#include <fstream>


gfx::Sprite_atlas::Sprite_atlas(rms::Resource_manager *pcreator, const char *name) : 
	rms::Resource(pcreator, name), m_atlas_file_path(nullptr), m_texture_file_path(nullptr) {}

void gfx::Sprite_atlas::set_atlas_file_path(const char *file_path) 
{
	if (m_atlas_file_path != nullptr) {
		free(m_atlas_file_path);
	}
	m_atlas_file_path = strdup(file_path);
}

const char *gfx::Sprite_atlas::get_texture_file_path() const { return m_texture_file_path; }

gfx::Texture_2d *gfx::Sprite_atlas::get_texture() { return m_ptexture; }

void gfx::Sprite_atlas::load() 
{
	//process the tileset xml file
	std::ifstream	in;
	std::string		line;

	in.open(m_atlas_file_path);
	if (in) {
		std::getline(in, line); // xml header
#ifndef NDEBUG
		std::cout << "xml header: " << line << std::endl;
#endif // !NDEBUG
		while (std::getline(in, line) && (line.find("<TextureAtlas") == std::string::npos)) { //parse until find the <TextureAtlas line
			;
		}
		if (!in) { //did not find the <TextureAtlas line
			std::cerr << __FUNCTION__ << ": Error parsing file " << m_atlas_file_path << ", could not find <TextureAtlas header" << std::endl;
		}
		else {
#ifndef NDEBUG
			std::cout << "Parsing atlas:" << std::endl << line << std::endl;
#endif // !NDEBUG
			//get the texture atlas file name 
			std::string	texture_file;
			std::string	aux;
			auto pos = line.find("imagePath");
			pos = line.find_first_of("\"", pos);
			++pos;
			texture_file = line.substr(pos, line.find_first_of("\"", pos) - pos);
			//concatenate the name with the currend directorie to have the full path 
			aux = m_atlas_file_path;
			pos = (aux.find_last_of("/") == std::string::npos) ? (aux.find_last_of("\\")) : (aux.find_last_of("/"));
			++pos;
			texture_file = aux.replace(pos, aux.size() - pos, texture_file);
#ifndef NDEBUG
			std::cout << __FILE__ << ": Full path to texture atlas: " << texture_file << std::endl;
#endif // !NDEBUG

			std::string digits("0123456789");
			//get atlas witdth and height
			pos = line.find("width");
			//m_image_size.x = std::stoi(line.substr(line.find_first_of(digits, pos)));

			pos = line.find("height");
			//m_image_size.y = std::stoi(line.substr(line.find_first_of(digits, pos)));

			//loop trough all the sprite lines
			while (std::getline(in, line) && (line.find("<sprite") != std::string::npos)) { // for every sprite int the atlas
				float x, y, w, h;
				//	std::cout << line << std::endl;
				pos = line.find("x");
				x = std::stoi(line.substr(line.find_first_of(digits, pos)));
				pos = line.find("y");
				y = std::stoi(line.substr(line.find_first_of(digits, pos)));
				pos = line.find("w", pos);
				w = std::stoi(line.substr(line.find_first_of(digits, pos)));;
				pos = line.find("h", pos);
				h = std::stoi(line.substr(line.find_first_of(digits, pos)));

				//std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;;

				math::Rect rect(x, y, w, h);
				m_vrec.push_back(rect);
			}
			// load the texture
			//m_texture = Texture_2d(texture_file);
			//set this atlas texture file path

			if (m_texture_file_path != nullptr) {
				free(m_texture_file_path);
			}

			m_texture_file_path = strdup(texture_file.c_str());
		}
	}
	else {
		std::cerr << __FUNCTION__ << ": could not open file " << m_atlas_file_path << std::endl;
	}
}

size_t gfx::Sprite_atlas::calculate_size() const 
{
	return sizeof(Sprite_atlas);
}

void gfx::Sprite_atlas::unload() 
{
	remove_string_id_entry(m_id);
	free(m_atlas_file_path);
	free(m_texture_file_path);
}

/* Reads the xml file containing the coordinates of the sprites in the atlas and 
 * the directory of the image file and, loads the texture object and the sprite atlas data
 */
/*
gfx::Sprite_atlas::Sprite_atlas(const std::string & file_name, const string_id id) : m_id(id)
{

	//process the tileset xml file
	std::ifstream	in;
	std::string		line;

	in.open(file_name);
	if (in) {
		std::getline(in, line); // xml header
#ifndef NDEBUG
		std::cout << "xml header: " << line << std::endl;
#endif // !NDEBUG
		while (std::getline(in, line) && (line.find("<TextureAtlas") == std::string::npos)) { //parse until find the <TextureAtlas line
			;
		}
		if (!in) { //did not find the <TextureAtlas line
			std::cerr << __FUNCTION__ << ": Error parsing file " << file_name << ", could not find <TextureAtlas header" << std::endl;
		}
		else {
#ifndef NDEBUG
			std::cout << "Parsing atlas:" << std::endl << line << std::endl;
#endif // !NDEBUG
			//get the texture atlas file name 
			std::string	texture_file;
			std::string	aux;
			auto pos = line.find("imagePath");
			pos = line.find_first_of("\"", pos);
			++pos;
			texture_file = line.substr(pos, line.find_first_of("\"", pos) - pos);
			//concatenate the name with the currend directorie to have the full path 
			aux = file_name;
			pos = (aux.find_last_of("/") == std::string::npos) ? (aux.find_last_of("\\")) : (aux.find_last_of("/"));
			++pos;
			texture_file = aux.replace(pos, aux.size() - pos, texture_file);
#ifndef NDEBUG
			std::cout << __FILE__ << ": Full path to texture atlas: " << texture_file << std::endl;
#endif // !NDEBUG

			std::string digits("0123456789");
			//get atlas witdth and height
			pos = line.find("width");
			//m_image_size.x = std::stoi(line.substr(line.find_first_of(digits, pos)));

			pos = line.find("height");
			//m_image_size.y = std::stoi(line.substr(line.find_first_of(digits, pos)));

			//loop trough all the sprite lines
			while (std::getline(in, line) && (line.find("<sprite") != std::string::npos)) { // for every sprite int the atlas
				float x, y, w, h;
			//	std::cout << line << std::endl;
				pos = line.find("x");
				x = std::stoi(line.substr(line.find_first_of(digits, pos)));
				pos = line.find("y");
				y = std::stoi(line.substr(line.find_first_of(digits, pos)));
				pos = line.find("w", pos);
				w = std::stoi(line.substr(line.find_first_of(digits, pos)));;
				pos = line.find("h", pos);
				h = std::stoi(line.substr(line.find_first_of(digits, pos)));

				//std::cout << "x=" << x << " y=" << y << " w=" << w << " h=" << h << std::endl;;

				math::Rect rect(x, y, w, h);
				m_vrec.push_back(rect);
			}
			// load the texture
			m_texture = Texture_2d(texture_file);
		}
	}
	else {
		std::cerr << __FUNCTION__ << ": could not open file " << file_name << std::endl;
	}

}*/

//get the uv coordinates for the sprite number 'sprite_no' in the texture atlas
void gfx::Sprite_atlas::get_text_coord(const int sprite_no, math::Rect *prect, float * px_width, float * px_height) const
{
	
//	cgm::vec2 uv0, uv1, uv2, uv3;
	const math::Rect & rect = m_vrec[sprite_no];

//	uv0.x = rect.x / m_image_size.x;
//	uv0.y = rect.y / m_image_size.y;

//	uv1.x = uv0.x;
//	uv1.y = uv0.y + rect.height / m_image_size.y;

//	uv2.x = uv0.x + rect.width / m_image_size.x;
//	uv2.y = uv1.y;

//	uv3.x = uv2.x;
//	uv3.y = uv0.y;
	prect->x       = rect.x / m_ptexture->get_width();
	prect->y       =  rect.y / m_ptexture->get_height();
	prect->width   =  rect.width / m_ptexture->get_width();
	prect->height  =  rect.height / m_ptexture->get_height();
	
	*px_width  = rect.width;
	*px_height = rect.height;

	//return Rect(rect.x / m_image_size.x, rect.y / m_image_size.y, rect.width / m_image_size.x, rect.height / m_image_size.y);

	/*
	m_vertices_uv.clear();
	m_vertices_uv.push_back(uv1);
	m_vertices_uv.push_back(uv2);
	m_vertices_uv.push_back(uv3);

	m_vertices_uv.push_back(uv3);
	m_vertices_uv.push_back(uv0);
	m_vertices_uv.push_back(uv1);
	*/
}

/*
gfx::Sprite_atlas::~Sprite_atlas() 
{
	//erase the id of the atlas
	remove_string_id_entry(m_id);
}*/