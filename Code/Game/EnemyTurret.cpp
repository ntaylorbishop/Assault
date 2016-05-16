#include "Game/EnemyTurret.hpp"

EnemyTurret::EnemyTurret(const WorldCoords& position, float orientation)
	: Entity(ENTITY_TURRET)
	, m_fireRateAge(0.f)
{
	m_texture = Texture::CreateOrGetTexture(textureToUse);
	m_position = position;
	m_orientation = orientation;
	m_physicsRadius = TURRET_PHYSICS_RADIUS;
	m_cosmeticRadius = TURRET_COSMETIC_RADIUS;
	m_quad = AABB2(Vector2(-m_cosmeticRadius, -m_cosmeticRadius), Vector2(m_cosmeticRadius, m_cosmeticRadius));

	m_health = TURRET_HEALTH;
}
EnemyTurret::~EnemyTurret() { }

void EnemyTurret::Update(float deltaSeconds) {
	m_fireRateAge += deltaSeconds;

	if (playerInLOS() && DistanceBetweenPoints(g_theWorld->m_playerTank.getPosition(), m_position) < TURRET_SIGHT_RADIUS && g_theWorld->m_playerTank.isAlive()) {
		rotate(deltaSeconds);
		fireBullet();
	}
}
void EnemyTurret::Render() const {
	g_theRenderer->PushViewMatrix();

	g_theRenderer->TranslateViewMatrix(m_position);
	g_theRenderer->RotateViewMatrix(m_orientation);
	g_theRenderer->DrawTexturedAABB2(m_texture, RGBA(1.f, 1.f, 1.f, 1.f), m_quad);

	g_theRenderer->PopViewMatrix();
}

bool EnemyTurret::playerInLOS() {
	return Raycast2DToPoint(&g_theWorld->m_gameMap, m_position, g_theWorld->m_playerTank.getPosition(), NULL);
}

void EnemyTurret::rotate(float deltaSeconds) {
	Vector2 displacementToPlayer = g_theWorld->m_playerTank.getPosition() - m_position;
	float angleToPlayer = atan2(displacementToPlayer.y, displacementToPlayer.x);
	float angularDisplacement = CalcShortestAngularDisplacement(ToDegrees(m_orientation), ToDegrees(angleToPlayer));

	if (abs(angularDisplacement) > TURRET_ANGULAR_VELOCITY + 2.f) {
		if (angularDisplacement > 0) {
			m_orientation += TURRET_ANGULAR_VELOCITY * deltaSeconds;
		}
		else if (angularDisplacement < 0) {
			m_orientation -= TURRET_ANGULAR_VELOCITY * deltaSeconds;
		}
	}
}
void EnemyTurret::fireBullet() {
	Vector2 dirToPlayer = g_theWorld->m_playerTank.getPosition() - m_position;
	dirToPlayer.normalize();

	Vector2 forwardVector = Vector2(cos(m_orientation), sin(m_orientation));

	if (DotProduct(dirToPlayer, forwardVector) >= TURRET_CONE_TOLERANCE) { //Within firing LOS
		if (m_fireRateAge > TURRET_FIRE_RATE) {
			m_fireRateAge = 0.f;

			g_theWorld->PlaySound(EFFECT_ENEMY_TURRET_SHOOT);
			
			Vector2 bulletPosition = m_position + Vector2(m_physicsRadius * 1.5f * cos(m_orientation), m_physicsRadius * 1.5f * sin(m_orientation));
			Bullet* b = new Bullet(bulletPosition, Vector2(TURRET_BULLET_SPEED, TURRET_BULLET_SPEED), Vector2(0.f, 0.f), m_orientation, TURRET_BULLET_SIZE, false);
			g_theWorld->m_entities.insert(b);
		}
	}
}