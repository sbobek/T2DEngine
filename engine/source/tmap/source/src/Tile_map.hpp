#ifndef _TILE_MAP_HPP

#define _TILE_MAP_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <cassert>

#include "Tileset.hpp"
#include "vec3.hpp"


//------------------- TILE MAP---------------------------------------------------------//
/* This class defines a data structure to store a map of 2d tiles, it reads its data from a tmx file.
 *  -m_tilestes stores a vector of tilesets, each tileset, contains a array of tiles.
 *  -m_map is a 3d array of tiles, the third dimension is for each 2D layer in the tile map
 *
 * Tile_map space is a left-right top-bottom coordinate space
 */

struct Tile;
namespace math { struct Rect; }

class Tile_map {
friend std::ostream & print_tile_map(std::ostream & os, const Tile_map & map);
public:
	Tile_map() = default;
	Tile_map(const std::vector<Tileset> & tilesets, const std::vector<std::vector<std::vector<int>>> & layers ,const int layer_count, const int width, const int height, const int tile_width, const int tile_height) : m_pixels_per_word_unit(16)
	{
		m_tilesets      =  tilesets;
		m_layer_count   =  layer_count;
		m_width         =  width;
		m_height        =  height;
		m_tile_width    =  tile_width;
		m_tile_height   =  tile_height;
		setup_map(layers);
	}
	int layer_count()  const { return m_layer_count; }
	int width()        const { return m_width; }
	int height()       const { return m_height; }
	int tile_width()   const { return m_tile_width;}
	int tile_height()  const { return m_tile_height; }
	const math::vec3 & get_background_color() const { return m_background_color; }
	std::vector<Tileset> & get_tilesets()  { return m_tilesets; }
	int get_tile_id(const int layer, const int row, const int column) const;
	Tile get_tile(const unsigned id) const;
	float pixels_per_world_unit() const { return m_pixels_per_word_unit; }

	const math::vec3 & get_position() const { return m_position; }

	math::Rect tile_wld_space_bounds(const unsigned row, const unsigned column) const;
	std::pair<float, float> wld_to_tile_space(const math::vec3 & pos) const;

	float world_to_tile_displacement_x(float t_x) const { return t_x / (m_tile_width / m_pixels_per_word_unit) ; }
	float world_to_tile_displacement_y(float t_y) const { return t_y / (m_tile_height / m_pixels_per_word_unit); }

	void set_background_color(const math::vec3 & color) { m_background_color = color; }
private:
	void setup_map(const std::vector<std::vector<std::vector<int>>> & layers);

	std::vector<Tileset> m_tilesets;
	std::vector<std::vector<std::vector<unsigned>>>  m_map; // 3D array, i.e layers of 2d arrays	 
	
	int     m_layer_count; // number of layers
	int     m_width;
	int     m_height;
	int     m_tile_width;
	int     m_tile_height;

	math::vec3 m_position;  // tile map origin, i.e, the coordinates of the top left tile in world space
	math::vec3 m_background_color;
	float     m_pixels_per_word_unit; // how many pixels are equal to 1 world unit, used in scaling
};

inline int Tile_map::get_tile_id(const int layer, const int row, const int column) const 
{
	//assert(row < m_height && "invalid row! outside map border");
	//assert(column < m_width && "invadlid column! outside maps border");
	return m_map[layer][row][column];
}


std::ostream & print_tile_map(std::ostream & os, const Tile_map & tile_map);

#endif // !_TILE_MAP_HPP