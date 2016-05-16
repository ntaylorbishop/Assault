#pragma once

#include <stdlib.h>
#include <vector>
#include <set>

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"

#include "Game/Entity.hpp"
#include "Game/Tile.hpp"

typedef Vector2 WorldCoords;
typedef IntVector2 TileCoords;
typedef int TileIndex;

using namespace std;
class Map {
public:
	Map(IntVector2 size);

	void Update(float deltaSeconds);
	void Render() const;

	//TileCoords getTileCoordsForTile(const Tile& tile) const;
	Tile* getTileAtTileCoords(const TileCoords& tileCoords);
	Tile& getTileAtIndex(TileIndex idx);
	const Tile* getTileAtWorldCoords(const WorldCoords& WorldCoords) const;
	Tile* getTileAtWorldCoords(const WorldCoords& WorldCoords);
	TileCoords getTileCoordsForWorldCoords(const WorldCoords& WorldCoords) const;
	TileCoords getTCForIndex(TileIndex idx) const;
	TileIndex getIndexForTC(const TileCoords& coords) const;

	void pushTile(const TileCoords& pos, TileType type);
	void setSize(IntVector2 size);
	void clearMap();

private:
	char* level1Path = "Data/Map/Level1.png";

	vector<Tile> m_tiles;
	TileCoords m_size;
	set<Entity*> m_entities;
};