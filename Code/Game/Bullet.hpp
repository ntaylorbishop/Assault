#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Game/Entity.hpp"

const float BULLET_PHYSICS_RADIUS = 0.2f;

class Bullet : public Entity {
public:
	Bullet();
	Bullet(WorldCoords position, Vector2 velocity, Vector2 acceleration, float orientation, float cosmeticRadius, bool belongsToPlayer);
	~Bullet();

	void Update(float deltaSeconds);
	void Render() const;

	bool belongsToPlayer();

private:
	char* textureToUse = "Data/Images/Bullet2.png";
	bool m_belongsToPlayer;

	//INHERITED FROM ENTITY
	// 	Texture* m_texture;

	// 	AABB2 m_quad;
	// 	WorldCoords m_position; //Center of PlayerTank
	// 	Vector2 m_acceleration;
	// 	float m_orientation;
	// 	float m_physicsRadius;
	// 	float m_cosmeticRadius;
	//  int EntityType = 1
};