#pragma once

#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Renderer/Spritesheet.hpp"

enum TileType {
	TILE_GRASS = 0,
	TILE_STONE,
	TILE_ROCK,
	TILE_DESERT
};

class TileDefinition {
public:
	//STRUCTORS
	TileDefinition();
	TileDefinition(TileType type);
	~TileDefinition();

	//ACCESSORS
	SpriteSheet* getSpriteSheet() const;
	IntVector2 getSpriteCoords() const;
	RGBA getColor() const;
	TileType getType() const;
	bool isSolid() const;
private:
	TileType m_tileType;

	char* m_spritesheetPath = "Data/Images/Tile_Spritesheet.png";
	IntVector2 m_spriteCoords;
	SpriteSheet* m_textures;
	RGBA m_color;
	bool solid;
};