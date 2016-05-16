#pragma once

#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"

typedef Vector2 WorldCoords;

enum EntityType {
	ENTITY_BULLET = -1,
	ENTITY_PLAYER,
	ENTITY_TANK,
	ENTITY_TURRET
};

class Entity {
public:
	//STRUCTORS
	Entity(EntityType type);
	~Entity();

	//UPDATE RENDER
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;

	//ACCESSORS
	Texture* getTexture() const;
	AABB2 getQuad() const;
	WorldCoords getPosition() const;
	Vector2 getVelocity() const;
	Vector2 getAcceleration() const;
	float getOrientation() const;
	float getPhysicsRadius() const;
	float getCosmeticRadius() const;
	int getType() const;
	int isAlive() const;
	int getHealth() const;

	//MUTATORS
	void setTexture(Texture* texture);
	void setQuad(const AABB2& quad);
	void setPosition(const Vector2& position);
	void setVelocity(const Vector2& velocity);
	void setAcceleration(const Vector2& acceleration);
	void setOrientation(const float orientation);
	void setPhysicsRadius(const float physicsRadius);
	void setCosmeticRadius(const float cosmeticRadius);

	void incrementHealth();
	void decrementHealth();
	void setHealth(int health);

	//Operator overloads
	bool Entity::operator==(const Entity &other) const {
		return (m_texture == other.m_texture &&
			    m_position == other.m_position &&
			    m_velocity == other.m_velocity &&
			    m_acceleration == other.m_acceleration &&
			    m_orientation == other.m_orientation &&
			    m_physicsRadius == other.m_physicsRadius &&
			    m_cosmeticRadius == other.m_cosmeticRadius &&
			    m_entityType == other.m_entityType);
	}
	bool Entity::operator!=(const Entity &other) const {
		return !(*this == other);
	}

protected:
	Texture* m_texture;

	AABB2 m_quad;
	WorldCoords m_position; //Center of quad
	Vector2 m_velocity;
	Vector2 m_acceleration;
	float m_orientation;
	float m_physicsRadius;
	float m_cosmeticRadius;
	int m_health;

	//ENTITY TYPE
	//-1 - Bullet
	//0 - Entity
	//1 - Player Tank
	//2 - Enemy Tank
	//3 - Enemy Turret
	//
	EntityType m_entityType;
};
