#include "Game/PlayerTank.hpp"

//----------------------------------STRUCTORS--------------------------------------

PlayerTank::PlayerTank() 
	: Entity(ENTITY_PLAYER)
{
	m_texture = nullptr;
	m_cameraView = Vector2(0.f, 0.f);
	m_position = Vector2(0.f, 0.f);
	m_orientation = 0.f;

	m_health = PLAYER_HEALTH;

	dashingLeft = false;
	dashingRight = false;
	dashingUp = false;
	dashingDown = false;
	m_dashingAge = 0.f;
}
PlayerTank::PlayerTank(const WorldCoords& position, float orientation, float physicsRadius) 
	: Entity(ENTITY_PLAYER)
{
	m_texture = Texture::CreateOrGetTexture(textureToUse);
	m_position = position;
	m_cameraView = position;
	m_orientation = orientation;
	m_physicsRadius = physicsRadius;

	m_health = PLAYER_HEALTH;

	dashingLeft = false;
	dashingRight = false;
	dashingUp = false;
	dashingDown = false;
	m_dashingAge = 0.f;
}
PlayerTank::~PlayerTank() { }

//----------------------------------UPDATE / RENDER--------------------------------------

void PlayerTank::Update(float deltaSeconds) {
	WorldCoords mins(m_cameraView.x - m_physicsRadius, m_cameraView.y - m_physicsRadius);
	WorldCoords maxs(m_cameraView.x + m_physicsRadius, m_cameraView.y + m_physicsRadius);
	m_quad = AABB2(mins, maxs);

	deltaSeconds = deltaSeconds;

	if (dashingLeft || dashingRight || dashingUp || dashingDown) {
		m_dashingAge += deltaSeconds;
		if (m_dashingAge >= DASH_AGE) {
			m_dashingAge = 0.f;
			dashingLeft = false;
			dashingRight = false;
			dashingUp = false;
			dashingDown = false;
		}
		else if (dashingLeft) {
			m_position = m_position + Vector2(DASH_SPEED * deltaSeconds * cos(m_orientation + pi / 2.f), DASH_SPEED * deltaSeconds * sin(m_orientation + pi / 2.f));
		}
		else if (dashingRight)
			m_position = m_position + Vector2(DASH_SPEED * deltaSeconds * cos(m_orientation - pi / 2.f), DASH_SPEED * deltaSeconds * sin(m_orientation - pi / 2.f));
		else if (dashingUp)
			m_position = m_position + Vector2(DASH_SPEED * deltaSeconds * cos(m_orientation), DASH_SPEED * deltaSeconds * sin(m_orientation));
		else if (dashingDown)
			m_position = m_position - Vector2(DASH_SPEED * deltaSeconds * cos(m_orientation), DASH_SPEED * deltaSeconds * sin(m_orientation));
	}
}
void PlayerTank::Render() const {
	g_theRenderer->DrawTexturedAABB2(m_texture, RGBA(1.f, 1.f, 1.f, 1.f), m_quad);
}

//----------------------------------MOVEMENT--------------------------------------
void PlayerTank::moveForward(float forward_speed) {
	m_position = m_position + Vector2(forward_speed * cos(m_orientation), forward_speed * sin(m_orientation));
}
void PlayerTank::moveBackward(float backward_speed) {
	m_position = m_position + Vector2(backward_speed * cos(m_orientation), backward_speed * sin(m_orientation));
}
void PlayerTank::rotateLeft(float rotation_strength) {
	m_orientation += rotation_strength;
}
void PlayerTank::rotateRight(float rotation_strength) {
	m_orientation -= rotation_strength;
}

//DASHING----------------------------------------------------------------------------------
void PlayerTank::dash(dashDirection dir) {
	if (dir == DASH_LEFT)
		dashingLeft = true;
	else if (dir == DASH_RIGHT)
		dashingRight = true;
	else if (dir == DASH_UP)
		dashingUp = true;
	else if (dir == DASH_DOWN)
		dashingDown = true;
}

//HEALTH-------------------------------------------------------------------------------------
int PlayerTank::getMaxHealth() {
	return PLAYER_HEALTH;
}