#include "Game/Bullet.hpp"

Bullet::Bullet() 
	: Entity(ENTITY_BULLET) 
{

}
Bullet::Bullet(WorldCoords position, Vector2 velocity, Vector2 acceleration, float orientation, float cosmeticRadius, bool belongsToPlayer) 
	: Entity(ENTITY_BULLET)
{
	m_texture = Texture::CreateOrGetTexture(textureToUse);
	m_position = position;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_orientation = orientation;
	m_physicsRadius = BULLET_PHYSICS_RADIUS;
	m_cosmeticRadius = cosmeticRadius;
	m_belongsToPlayer = belongsToPlayer;

	m_quad = AABB2(Vector2(-m_cosmeticRadius, -m_cosmeticRadius), Vector2(m_cosmeticRadius, m_cosmeticRadius));
}

Bullet::~Bullet() {

}

void Bullet::Update(float deltaSeconds) {
	m_position = m_position + Vector2(m_velocity.x * cos(m_orientation) * deltaSeconds, m_velocity.y * sin(m_orientation) * deltaSeconds);
}
void Bullet::Render() const {
	g_theRenderer->PushViewMatrix();
	g_theRenderer->TranslateViewMatrix(m_position);
	g_theRenderer->RotateViewMatrix(m_orientation);

	if (m_texture == nullptr)
		g_theRenderer->DrawAABB2(m_quad, RGBA(1.f, 1.f, 1.f, 1.f), PRIMITIVE_QUADS);
	else
		g_theRenderer->DrawTexturedAABB2(m_texture, RGBA(1.f, 1.f, 1.f, 1.f), m_quad);

	g_theRenderer->PopViewMatrix();
}

bool Bullet::belongsToPlayer() {
	return m_belongsToPlayer;
}