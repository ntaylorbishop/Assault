#include "Game/Entity.hpp"

//----------------------------------STRUCTORS--------------------------------------
Entity::Entity(EntityType type)
	: m_texture(nullptr)
	, m_quad(Vector2(0.f, 0.f), Vector2(0.f, 0.f))
	, m_position(0.f, 0.f)
	, m_velocity(0.f, 0.f)
	, m_acceleration(0.f, 0.f)
	, m_orientation(0.f)
	, m_physicsRadius(0.f)
	, m_cosmeticRadius(0.f)
	, m_health(0)
	, m_entityType(type)
{
}
Entity::~Entity() {

}

//----------------------------------ACCESSORS--------------------------------------
Texture* Entity::getTexture() const {
	return m_texture;
}
AABB2 Entity::getQuad() const {
	return m_quad;
}
WorldCoords Entity::getPosition() const {
	return m_position;
}
Vector2 Entity::getVelocity() const {
	return m_velocity;
}
Vector2 Entity::getAcceleration() const {
	return m_acceleration;
}
float Entity::getOrientation() const {
	return m_orientation;
}
float Entity::getPhysicsRadius() const {
	return m_physicsRadius;
}
float Entity::getCosmeticRadius() const {
	return m_cosmeticRadius;
}
int Entity::getType() const {
	return m_entityType;
}
int Entity::isAlive() const {
	return (m_health > 0);
}
int Entity::getHealth() const {
	return m_health;
}

//----------------------------------MUTATORS--------------------------------------
void Entity::setTexture(Texture* texture) {
	m_texture = texture;
}
void Entity::setQuad(const AABB2& quad) {
	m_quad = quad;
}
void Entity::setPosition(const Vector2& position) {
	m_position = position;
}
void Entity::setVelocity(const Vector2& velocity) {
	m_velocity = velocity;
}
void Entity::setAcceleration(const Vector2& acceleration) {
	m_acceleration = acceleration;
}
void Entity::setOrientation(const float orientation) {
	m_orientation = orientation;
}
void Entity::setPhysicsRadius(const float physicsRadius) {
	m_physicsRadius = physicsRadius;
}
void Entity::setCosmeticRadius(const float cosmeticRadius) {
	m_cosmeticRadius = cosmeticRadius;
}

void Entity::incrementHealth() {
	m_health++;
}
void Entity::decrementHealth() {
	m_health--;
}
void Entity::setHealth(int health) {
	m_health = health;
}