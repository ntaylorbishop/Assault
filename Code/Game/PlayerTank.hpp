#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Engine/Math/AABB2.hpp"

#include "Game/Entity.hpp"

typedef Vector2 WorldCoords;

const int PLAYER_HEALTH = 10;
const float DASH_AGE = 0.2f;
const float DASH_SPEED = 10.f;

enum dashDirection {
	DASH_LEFT = 0,
	DASH_RIGHT,
	DASH_UP,
	DASH_DOWN
};  
//asdasdasd
class PlayerTank : public Entity {
public:
	//STRUCTORS
	PlayerTank();
	PlayerTank(const WorldCoords& position, float orientation, float physicsRadius);
	~PlayerTank();

	//UPDATE / RENDER
	void Update(float deltaSeconds);
	void Render() const;

	//MOVEMENT FUNCS
	void moveForward(float forward_speed);
	void moveBackward(float backward_speed);
	void rotateLeft(float rotation_strength);
	void rotateRight(float rotation_strength);

	void dash(dashDirection dir);

	int getMaxHealth();

private:
	char* textureToUse = "Data/Images/PlayerTank.png";
	bool dashingLeft;
	bool dashingRight;
	bool dashingUp;
	bool dashingDown;
	float m_dashingAge;

//INHERITED FROM ENTITY
// 	Texture* m_texture;

// 	AABB2 m_quad;
// 	WorldCoords m_position; //Center of PlayerTank
// 	Vector2 m_acceleration;
// 	float m_orientation;
// 	float m_physicsRadius;
// 	float m_cosmeticRadius;
//  int EntityType = 1

	Vector2 m_cameraView;
};