#include "Game/Raycast2D.hpp"

bool Raycast2DToPoint(const Map* gameMap, const Vector2& start, const Vector2& end, RaycastResult* out_result) {
	Vector2 displacement = end - start;
 	Vector2 current = start;
 	Vector2 step = displacement * 0.01f;

	for (int i = 0; i < 100; i++) {
		if (!gameMap->getTileAtWorldCoords(current)->isSolid()) {
			current = current + step;
		}
		else {
			if (out_result != NULL) {
				out_result->m_didImpact = true;
				out_result->m_impactPos = current;
				out_result->m_impactTile = gameMap->getTileAtWorldCoords(current);
			}
			return false;
		}
	}
	return true;
}