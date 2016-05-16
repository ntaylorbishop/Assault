#pragma once

#include <math.h>

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Raycast2D.hpp"
#include "Game/Entity.hpp"
#include "Game/PlayerTank.hpp"
#include "Game/Bullet.hpp"

const float TURRET_PHYSICS_RADIUS = 0.4f;
const float TURRET_COSMETIC_RADIUS = 0.4f;
const float TURRET_ANGULAR_VELOCITY = 2.f;
const float TURRET_FIRE_RATE = 1.f / 2.f;
const float TURRET_SIGHT_RADIUS = 10.f;
const float TURRET_BULLET_SPEED = 6.f;
const float TURRET_BULLET_SIZE = 0.2f;
const float TURRET_CONE_TOLERANCE = 0.9f;
const int TURRET_HEALTH = 1;

class EnemyTurret : public Entity {
public:
	EnemyTurret(const WorldCoords& position, float orientation);
	~EnemyTurret();

	void Update(float deltaSeconds);
	void Render() const;

	bool playerInLOS();

	void rotate(float deltaSeconds);
	void fireBullet();

private:

	char* textureToUse = "Data/Images/Turret.png";

	float m_fireRateAge;

	//INHERITED FROM ENTITY---------------------
	// 	Texture* m_texture;
	// 	AABB2 m_quad;
	// 	WorldCoords m_position; //Center of quad
	// 	Vector2 m_velocity;
	// 	Vector2 m_acceleration;
	// 	float m_orientation;
	// 	float m_physicsRadius;
	// 	float m_cosmeticRadius;
};