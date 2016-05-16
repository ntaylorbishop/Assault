#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Game/Map.hpp"
#include "Game/Tile.hpp"

struct RaycastResult {
	Vector2 m_impactPos;
	const Tile* m_impactTile;
	Vector2 m_impactSurfaceNormal; //Might not need this. Tells you what side of the tile it hit
	bool m_didImpact;
};

bool Raycast2DToPoint(const Map* gameMap, const Vector2& start, const Vector2& end, RaycastResult* out_result);
