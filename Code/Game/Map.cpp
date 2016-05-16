#include "Game/Map.hpp"

Map::Map(TileCoords size)
:m_size(size) 
{
	int chance = 0;
	for (int i = 0; i < m_size.x; i++) {
		for (int j = 0; j < m_size.y; j++) {
			//Add border tiles
			if (i == 0 || j == 0 || i == m_size.x - 1 || j == m_size.y - 1) {
				m_tiles.push_back(Tile(TileCoords(i, j), TILE_STONE));
				continue;
			}

			//Add 30% chance to add rock
			chance = rand() % 10 + 1;
			if (chance < 3 && j > 4) {
				m_tiles.push_back(Tile(TileCoords(i, j), TILE_ROCK));
				continue;
			}

			m_tiles.push_back(Tile(TileCoords(i, j), TILE_GRASS));
		}
	}
}

void Map::Update(float deltaSeconds) {
	deltaSeconds = 0;
}
void Map::Render() const {
	for (unsigned int i = 0; i < m_tiles.size(); i++) {
		m_tiles[i].Render();
	}
}

Tile* Map::getTileAtTileCoords(const TileCoords& tileCoords) {
	return &m_tiles[getIndexForTC(tileCoords)];
}
Tile& Map::getTileAtIndex(TileIndex idx) {
	return m_tiles[idx];
}
const Tile* Map::getTileAtWorldCoords(const WorldCoords& WorldCoords) const {
	const TileCoords TC = getTileCoordsForWorldCoords(WorldCoords);
	return &m_tiles[getIndexForTC(TC)];
}
Tile* Map::getTileAtWorldCoords(const WorldCoords& WorldCoords) {
	const TileCoords TC = getTileCoordsForWorldCoords(WorldCoords);
	return &m_tiles[getIndexForTC(TC)];
}
TileCoords Map::getTileCoordsForWorldCoords(const WorldCoords& WorldCoords) const {
	return TileCoords((int)floor(WorldCoords.x), (int)floor(WorldCoords.y));
}
TileCoords Map::getTCForIndex(TileIndex idx) const {
	int tileY = idx / m_size.x;
	int tileX = idx % m_size.x;

	return TileCoords(tileX, tileY);
}
TileIndex Map::getIndexForTC(const TileCoords& coords) const {
	TileIndex idx = m_size.x * coords.y + (m_size.x - coords.x);
	return idx;
}

void Map::pushTile(const TileCoords& pos, TileType type) {
	m_tiles.push_back(Tile(pos, type));
}
void Map::clearMap() {
	m_tiles.clear();
}
void Map::setSize(IntVector2 size) {
	m_size = size;
}