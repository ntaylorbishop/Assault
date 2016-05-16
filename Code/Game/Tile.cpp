#include "Game/Tile.hpp"

Tile::Tile()
{}
Tile::Tile(TileCoords loc, TileType type) {
	m_tileDef = TileDefinition(type);
	m_tileCoords = loc;
}

void Tile::Render() const {
	Vector2 mins = Vector2((float)m_tileCoords.x, (float)m_tileCoords.y);
	Vector2 maxs = Vector2((float)m_tileCoords.x, (float)m_tileCoords.y) + Vector2(1.f, 1.f);
	AABB2 quad(mins, maxs);

	g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_tileDef.getSpriteSheet(), m_tileDef.getColor(), quad, m_tileDef.getSpriteCoords());
	if (inDebug)
		g_theRenderer->DrawAABB2(quad, RGBA(0.f, 0.f, 0.f, 1.f), PRIMITIVE_LINE_LOOP);
}

TileType Tile::getType() const {
	return m_tileDef.getType();
}
TileCoords Tile::getTileCoords() const {
	return m_tileCoords;
}
WorldCoords Tile::getCenterInWorldCoords() {
	return WorldCoords((float)m_tileCoords.x + 0.5f, (float)m_tileCoords.y + 0.5f);
}

void Tile::setTileCoords(const TileCoords& tileCoords) {
	m_tileCoords = tileCoords;
}
bool Tile::isSolid() const {
	return m_tileDef.isSolid();
}