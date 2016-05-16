#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Game/TheWorld.hpp"
#include "Game/Entity.hpp"
#include "Game/Bullet.hpp"
#include "Game/Raycast2D.hpp"

const float TANK_PHYSICS_RADIUS = 0.4f;
const float TANK_COSMETIC_RADIUS = 0.4f;
const float TANK_ORIENTATION_TIME_CHANGE = 3.f;
const float TANK_SIGHT_RADIUS = 10.f;
const float TANK_ANGULAR_VELOCITY = 10.f;
const float TANK_FORWARD_SPEED = 1.f;
const float TANK_CHASING_SPEED = 3.f;
const float TANK_CONE_TOLERANCE = 0.9f;
const float TANK_FIRE_RATE = 1.f; //Four times a second
const float TANK_BULLET_SPEED = 6.f;
const float TANK_BULLET_SIZE = 0.2f;
const int TANK_HEALTH = 2;

class EnemyTank : public Entity {
public:
	EnemyTank(const WorldCoords& position, float orientation);
	~EnemyTank();

	void Update(float deltaSeconds);
	void Render() const;

	bool playerInLOS();

	void moveForward(float deltaSeconds);
	void rotate(float deltaSeconds, float goalOrientation);
	void fireBullet();

private:
	char* textureToUse = "Data/Images/Tank2.png";

	float m_orientationAge;
	float m_goalOrientation;
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