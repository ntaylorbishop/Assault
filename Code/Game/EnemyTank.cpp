#include "Game/EnemyTank.hpp"

EnemyTank::EnemyTank(const WorldCoords& position, float orientation)
	: Entity(ENTITY_TANK)
	, m_orientationAge(0.f)
	, m_goalOrientation(0.f)
	, m_fireRateAge(0.f)
{
	m_texture = Texture::CreateOrGetTexture(textureToUse);
	m_position = position;
	m_orientation = orientation;
	m_physicsRadius = TANK_PHYSICS_RADIUS;
	m_cosmeticRadius = TANK_COSMETIC_RADIUS;
	m_quad = AABB2(Vector2(-m_cosmeticRadius, -m_cosmeticRadius), Vector2(m_cosmeticRadius, m_cosmeticRadius));

	m_orientation = Randf(-2.f * pi, 2.f * pi);

	m_health = TANK_HEALTH;
}
EnemyTank::~EnemyTank() { }

void EnemyTank::Update(float deltaSeconds) {
	m_orientationAge += deltaSeconds;
	m_fireRateAge += deltaSeconds;

	//Change orientation every three seconds
	if (DistanceBetweenPoints(m_position, g_theWorld->m_playerTank.getPosition()) < TANK_SIGHT_RADIUS && g_theWorld->m_playerTank.isAlive() && playerInLOS()) {

		Vector2 displacementToPlayer = g_theWorld->m_playerTank.getPosition() - m_position;
		float angleToPlayer = atan2(displacementToPlayer.y, displacementToPlayer.x);

		rotate(deltaSeconds, angleToPlayer);
		fireBullet();
	}
	else {
		//Change orientation
		if (m_orientationAge > TANK_ORIENTATION_TIME_CHANGE) {

			m_orientationAge = 0.f;
			m_goalOrientation = Randf(-2.f * pi, 2.f * pi);

		}

		rotate(deltaSeconds, m_goalOrientation);
	}

	moveForward(deltaSeconds);

}
void EnemyTank::Render() const {
	g_theRenderer->PushViewMatrix();

	g_theRenderer->TranslateViewMatrix(m_position);
	g_theRenderer->RotateViewMatrix(m_orientation);
	g_theRenderer->DrawTexturedAABB2(m_texture, RGBA(1.f, 0.2f, 0.2f, 1.f), m_quad);

	g_theRenderer->PopViewMatrix();
}

//MOVEMENT STUFF--------------------------------------------------------------
void EnemyTank::moveForward(float deltaSeconds) {
	m_position = m_position + Vector2(TANK_FORWARD_SPEED * cos(m_orientation) * deltaSeconds, TANK_FORWARD_SPEED * sin(m_orientation) * deltaSeconds);
}
void EnemyTank::rotate(float deltaSeconds, float goalOrientation) {
	float angularDisplacement = CalcShortestAngularDisplacement(ToDegrees(m_orientation), ToDegrees(goalOrientation));

	if (abs(angularDisplacement) > TANK_ANGULAR_VELOCITY + 2.f) { //2.f is tolerance to reduce entity stutter
		if (angularDisplacement > 0)
			m_orientation += TANK_ANGULAR_VELOCITY * deltaSeconds;
		else if (angularDisplacement < 0)
			m_orientation -= TANK_ANGULAR_VELOCITY * deltaSeconds;
	}
}
void EnemyTank::fireBullet() {

	Vector2 dirToPlayer = g_theWorld->m_playerTank.getPosition() - m_position;
	dirToPlayer.normalize();

	Vector2 forwardVector = Vector2(cos(m_orientation), sin(m_orientation));

	if (DotProduct(dirToPlayer, forwardVector) >= TANK_CONE_TOLERANCE) { //Within firing LOS
		if (m_fireRateAge > TANK_FIRE_RATE) {
			m_fireRateAge = 0.f;

			g_theWorld->PlaySound(EFFECT_ENEMY_TANK_SHOOT);

			Vector2 bulletPosition = m_position + Vector2(m_physicsRadius * 1.5f * cos(m_orientation), m_physicsRadius * 1.5f * sin(m_orientation));
			Bullet* b = new Bullet(bulletPosition, Vector2(TANK_BULLET_SPEED, TANK_BULLET_SPEED), Vector2(0.f, 0.f), m_orientation, TANK_BULLET_SIZE, false);
			g_theWorld->m_entities.insert(b);
		}
	}
}

//UTILITY---------------------------------------------------------------------
bool EnemyTank::playerInLOS() {
	return Raycast2DToPoint(&g_theWorld->m_gameMap, m_position, g_theWorld->m_playerTank.getPosition(), NULL);
}