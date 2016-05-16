#include "Game/TileDefinition.hpp"

//STRUCTORS------------------------------------------------------------------------
TileDefinition::TileDefinition()
    : m_color(RGBA())
    , m_tileType(TILE_GRASS)
	, m_textures(nullptr)
	, m_spriteCoords()
{}
TileDefinition::TileDefinition(TileType type)
    : m_color(RGBA())
    , m_tileType(TILE_GRASS)
	, m_textures(nullptr)
	, m_spriteCoords()
{
	m_tileType = type;
	m_textures = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(2, 2), m_spritesheetPath);

	if (m_tileType == TILE_GRASS) {
		m_color = RGBA(0x12, 0x95, 0x00, 0xFF);
		m_spriteCoords = IntVector2(1, 0); //Grass texture
		solid = false;
	}
	else if (m_tileType == TILE_ROCK) {
		m_color = RGBA(0x9B, 0x9A, 0x9A, 0xFF);
		m_spriteCoords = IntVector2(0, 0); //Stone texture
		solid = true;
	}
	else if (m_tileType == TILE_STONE) {
		m_color = RGBA(0x43, 0x43, 0x43, 0xFF);
		m_spriteCoords = IntVector2(0, 0); //Stone texture
		solid = true;
	}
	else if (m_tileType == TILE_DESERT) {
		m_color = RGBA(0x78, 0x5a, 0x00, 0xFF);
		m_spriteCoords = IntVector2(0, 1);
		solid = false;
	}
}
TileDefinition::~TileDefinition() { }

//ACCESSORS------------------------------------------------------------------------
SpriteSheet* TileDefinition::getSpriteSheet() const {
	return m_textures;
}

IntVector2 TileDefinition::getSpriteCoords() const {
	return m_spriteCoords;
}
RGBA TileDefinition::getColor() const {
	return m_color;
}
TileType TileDefinition::getType() const {
	return m_tileType;
}
bool TileDefinition::isSolid() const {
	return solid;
}