#pragma once

#include <string>

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/ErrorWarningAssert.hpp"
#include "Game/TileDefinition.hpp"

typedef IntVector2 TileCoords;
typedef Vector2 WorldCoords;

const float inDebug = false;

using namespace std;

class Tile {
public:
	Tile();
	Tile(TileCoords loc, TileType type);

	void Render() const;

	TileType getType() const;
	TileCoords getTileCoords() const;
	WorldCoords getCenterInWorldCoords();
	void setTileCoords(const TileCoords& tileCoords);
	bool isSolid() const;
private:
	TileDefinition m_tileDef;
	TileCoords m_tileCoords;

};