#pragma once

#include <stdlib.h>
#include <time.h>

#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Engine/Math/PolarCoords.hpp"
#include "Engine/XboxController.hpp"

#include "Game/Entity.hpp"
#include "Game/TheApp.hpp"
#include "Game/TheUI.hpp"
#include "Game/Camera2D.hpp"
#include "Game/Map.hpp"
#include "Game/PlayerTank.hpp"
#include "Game/Bullet.hpp"
#include "Game/EnemyTank.hpp"
#include "Game/EnemyTurret.hpp"

class TheGame;
extern TheGame* g_theGame;

typedef IntVector2 TileCoords;
typedef Vector2 WorldCoords;

//Consts for tweaking game
const IntVector2 MAP_SIZE = IntVector2(20, 30);
const Vector2 CAMERA_VIEW_PORT = Vector2(.5f, .2f);
const Vector2 SCREEN_SIZE = Vector2(16.f, 9.f);
const float KB_FORWARD_SPEED = 4.f;
const float KB_BACKWARD_SPEED = -4.f;
const float CONTROLLER_FORWARD_SPEED = 2.f;
const float CONTROLLER_BACKWARD_SPEED = -2.f;
const float KB_ROTATION_STRENGTH = 2.f;
const float CONTROLLER_ROTATION_STRENGTH = .9f;
const float CONTROLLER_ROTATION_STRENGTH_DIFF = .6f; //Strength of rotation when sticks are slightly off
const float BULLET_SPEED = 9.f;
const float PLAYER_BULLET_SIZE = 0.2f;
const float RAPID_FIRE_TIME = 1.f / 6.f;
const float ENTITY_SIZE = 0.4f;
const float PLAYER_DEATH_TIME = 3.f;
const float ANIM_SIZE_BULLET = 0.4f;
const float ANIM_SIZE_PLAYER_DEATH = 2.f;
const float ANIM_SIZE_ENEMY_DEATH = 1.5f;


using namespace std;
class TheGame {
public:
	//STRUCTORS
	TheGame();
	~TheGame();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//INPUT CHECKS
	void KB_InputCheck(float deltaSeconds);
	void Controller_InputCheck(float deltaSeconds);

	//COLLISION CHECKS
	void checkTileCollision(Entity* entity);
	void checkEntityCollision(Entity* entity);
	void checkBulletCollision();

	void pushEntityFromPoint(Entity* entity, Vector2 position);

	//OTHER
	void fireBullet(Entity* entity);

	void spawnEnemies();
	void killPlayer();
	void revivePlayer();

private:
	XboxController m_XboxController;

	float m_rapidFireAge;
	float m_deathAge;
	bool m_playerKilled;
};