#include "Game/TheGame.hpp"

TheGame* g_theGame = nullptr;

//----------------------------------STRUCTORS--------------------------------------
TheGame::TheGame()
	: m_rapidFireAge(0.f)
	, m_deathAge(0.f)
	, m_playerKilled(false)
{
	g_theWorld = new TheWorld(MAP_SIZE, SCREEN_SIZE, CAMERA_VIEW_PORT, ENTITY_SIZE);
	g_theUI = new TheUI(SCREEN_SIZE);
	m_XboxController = XboxController();
	srand((unsigned int)time(NULL)); //Seed RNG


	g_theWorld->PlayMusic(MUSIC_START_SCREEN);
	//spawnEnemies();
}
TheGame::~TheGame() { 
	delete g_theWorld;
	delete g_theUI;
}

//----------------------------------UPDATE / RENDER--------------------------------------
void TheGame::Update(float deltaSeconds) {
	float tmpDeltaSeconds = deltaSeconds;

	g_theWorld->Update(deltaSeconds);

	if (g_theUI->isPaused())
		deltaSeconds = 0.f;
	if (g_theWorld->m_playerTank.isAlive()) {
		if (g_theWorld->m_gameMap.getTileAtWorldCoords(g_theWorld->m_playerTank.getPosition())->getType() == TILE_DESERT && !g_theUI->inAttractMode()) {
			g_theUI->showWinScreen();
		}

		if (g_theUI->inWinScreen()) {
			DebuggerPrintf("%f\n", g_theUI->winScreenAge());
			deltaSeconds = deltaSeconds / (g_theUI->winScreenAge() * 5.f);

		}
	}
	else {
		m_deathAge += deltaSeconds;

		if (m_deathAge > PLAYER_DEATH_TIME) {
			m_deathAge = 0.f;
			g_theUI->pauseGame();
		}
	}

	m_rapidFireAge += deltaSeconds;
	KB_InputCheck(deltaSeconds);
	Controller_InputCheck(deltaSeconds);
	
	g_theWorld->m_playerTank.Update(deltaSeconds);

	//Cast playerTank to Entity* so we can check for collisions
	PlayerTank* playerTank = &g_theWorld->m_playerTank;
	Entity* playerTankEntity = dynamic_cast<Entity*>(playerTank);
	checkTileCollision(playerTankEntity);

	std::set<Entity*>::iterator it;
	for (it = g_theWorld->m_entities.begin(); it != g_theWorld->m_entities.end(); ++it) {
		Entity* entity = *it;
		entity->Update(deltaSeconds);
		if (entity->getType() != ENTITY_BULLET) {
			checkEntityCollision(entity);
			checkTileCollision(entity);
		}
	}

	checkBulletCollision();

	g_theWorld->m_mainCamera.MoveCamera(Vector2(-g_theWorld->m_playerTank.getPosition().x, -g_theWorld->m_playerTank.getPosition().y));
	g_theWorld->m_mainCamera.RotateCamera(g_theWorld->m_playerTank.getOrientation());

	g_theUI->Update(tmpDeltaSeconds);
}

void TheGame::Render() const {
	//Grid rotation
 	g_theRenderer->ClearColor(RGBA(0.f, 0.f, 0.f, 1.f));
 	g_theRenderer->ClearBuffer(GL_COLOR_BUFFER_BIT);
	g_theRenderer->LoadIdentity();
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	//Move camera
	g_theRenderer->TranslateViewMatrix(g_theWorld->m_mainCamera.GetViewPort());
	g_theRenderer->RotateViewMatrix(-g_theWorld->m_mainCamera.GetCameraRotation());
	g_theRenderer->TranslateViewMatrix(Vector2(g_theWorld->m_mainCamera.GetCameraPosition().x, g_theWorld->m_mainCamera.GetCameraPosition().y));

	g_theWorld->Render();
	g_theUI->Render();
}

//----------------------------------INPUT CHECKS--------------------------------------
void TheGame::KB_InputCheck(float deltaSeconds) {
	if (g_theWorld->m_playerTank.isAlive() && !g_theUI->isPaused()) {
		if (g_theApp->getKey(VK_LEFT)) {
			g_theWorld->m_mainCamera.RotateCamera(KB_ROTATION_STRENGTH * deltaSeconds);
			g_theWorld->m_playerTank.rotateLeft(KB_ROTATION_STRENGTH * deltaSeconds);
		}
		else if (g_theApp->getKey(VK_RIGHT)) {
			g_theWorld->m_mainCamera.RotateCamera(-KB_ROTATION_STRENGTH * deltaSeconds);
			g_theWorld->m_playerTank.rotateRight(KB_ROTATION_STRENGTH * deltaSeconds);
		}


		if (g_theApp->getKey(VK_UP)) {
			g_theWorld->m_playerTank.moveForward(KB_FORWARD_SPEED * deltaSeconds);
		}
		else if (g_theApp->getKey(VK_DOWN)) {
			g_theWorld->m_playerTank.moveBackward(KB_BACKWARD_SPEED * deltaSeconds);
		}

		if (g_theApp->getKey(VK_SPACE)) {
			if (m_rapidFireAge > RAPID_FIRE_TIME) {
				m_rapidFireAge = 0.f;
				fireBullet(&g_theWorld->m_playerTank);
			}
		}

		//Dashing
		if (g_theApp->getKeyDown('V'))
			g_theWorld->m_playerTank.dash(DASH_RIGHT);
		if (g_theApp->getKeyDown('C'))
			g_theWorld->m_playerTank.dash(DASH_LEFT);
	}

	//Different modes
	if (g_theApp->getKeyDown('P')) {
		if (g_theUI->inAttractMode()) {
			g_theWorld->m_playerTank.setHealth(g_theWorld->m_playerTank.getMaxHealth());
			g_theUI->leaveAttractMode();
		}
		else if (g_theUI->inWinScreen()) {
			g_theUI->disableWinScreen();
			g_theUI->goToAttractMode();
		}
		else if (g_theUI->isPaused()) {
			if (!g_theWorld->m_playerTank.isAlive()) {
				revivePlayer();
			}
			g_theUI->unpauseGame();
		}
		else {
			if (g_theWorld->m_playerTank.isAlive())
				g_theUI->pauseGame();
		}
	}

	if (g_theApp->getKeyDown(VK_ESCAPE)) {
		if (g_theUI->inAttractMode())
			g_theApp->quitGame();
		else if (g_theUI->inWinScreen()) {
			g_theUI->disableWinScreen();
			g_theUI->goToAttractMode();
		}
		else if (!g_theUI->isPaused())
			g_theUI->pauseGame();
		else if (g_theUI->isPaused())
			g_theUI->goToAttractMode();
	}
}
void TheGame::Controller_InputCheck(float deltaSeconds) {
	m_XboxController.Update();

	if (g_theWorld->m_playerTank.isAlive() && !g_theUI->isPaused()) {
		PolarCoords leftStickPolar = m_XboxController.getLeftStickPositionPolar();
		PolarCoords rightStickPolar = m_XboxController.getRightStickPositionPolar();

		if (leftStickPolar.radius > 0.f && rightStickPolar.radius > 0.f) { //Both sticks moving
			//MOVE FORWARD
			if (sin(leftStickPolar.theta) > 0 && sin(rightStickPolar.theta) > 0) { 
				float stickSpeed = (CONTROLLER_FORWARD_SPEED * leftStickPolar.radius) + (CONTROLLER_FORWARD_SPEED * leftStickPolar.radius);
				g_theWorld->m_playerTank.moveForward(stickSpeed * deltaSeconds);

				float sticksDiff = leftStickPolar.radius - rightStickPolar.radius;
				if (sticksDiff > 0)
					g_theWorld->m_playerTank.rotateRight(sticksDiff * deltaSeconds * CONTROLLER_ROTATION_STRENGTH_DIFF);
				else if (sticksDiff < 0)
					g_theWorld->m_playerTank.rotateLeft(-sticksDiff * deltaSeconds * CONTROLLER_ROTATION_STRENGTH_DIFF);
			}
			//MOVE BACKWARD
			else if (sin(leftStickPolar.theta) < 0 && sin(rightStickPolar.theta) < 0) {
				float stickSpeed = (CONTROLLER_BACKWARD_SPEED * leftStickPolar.radius) + (CONTROLLER_BACKWARD_SPEED * leftStickPolar.radius);
				g_theWorld->m_playerTank.moveBackward(stickSpeed * deltaSeconds);

				float sticksDiff = leftStickPolar.radius - rightStickPolar.radius;
				if (sticksDiff > 0)
					g_theWorld->m_playerTank.rotateRight(sticksDiff * deltaSeconds * CONTROLLER_ROTATION_STRENGTH_DIFF);
				else
					g_theWorld->m_playerTank.rotateLeft(sticksDiff * deltaSeconds * CONTROLLER_ROTATION_STRENGTH_DIFF);
			}
			//ROTATE RIGHT
			else if (sin(leftStickPolar.theta) > 0 && sin(rightStickPolar.theta) < 0) {
				float stickSpeed = (CONTROLLER_ROTATION_STRENGTH * leftStickPolar.radius) + (CONTROLLER_ROTATION_STRENGTH * leftStickPolar.radius);
				g_theWorld->m_playerTank.rotateRight(stickSpeed * deltaSeconds);
			}
			//ROTATE LEFT
			else if (sin(leftStickPolar.theta) < 0 && sin(rightStickPolar.theta) > 0) {
				float stickSpeed = (CONTROLLER_ROTATION_STRENGTH * leftStickPolar.radius) + (CONTROLLER_ROTATION_STRENGTH * leftStickPolar.radius);
				g_theWorld->m_playerTank.rotateLeft(stickSpeed * deltaSeconds);
			}
		}
		else { //One stick moving
			if (leftStickPolar.radius > 0.f && rightStickPolar.radius == 0.f) {
				float stickSpeed = CONTROLLER_FORWARD_SPEED * .2f * leftStickPolar.radius;
				if (sin(leftStickPolar.theta) < 0) {
					g_theWorld->m_playerTank.moveForward(stickSpeed * deltaSeconds);
					g_theWorld->m_playerTank.rotateLeft(stickSpeed * deltaSeconds * .5f);
				}
				else {
					g_theWorld->m_playerTank.moveForward(stickSpeed * deltaSeconds);
					g_theWorld->m_playerTank.rotateRight(stickSpeed * deltaSeconds * .5f);
				}
			}
			else if (leftStickPolar.radius == 0.f && rightStickPolar.radius > 0.f) {
				float stickSpeed = CONTROLLER_FORWARD_SPEED * .2f * rightStickPolar.radius;
				if (sin(rightStickPolar.theta) > 0) {
					g_theWorld->m_playerTank.moveForward(stickSpeed * deltaSeconds);
					g_theWorld->m_playerTank.rotateLeft(stickSpeed * deltaSeconds * .5f);
				}
				else {
					g_theWorld->m_playerTank.moveForward(stickSpeed * deltaSeconds);
					g_theWorld->m_playerTank.rotateRight(stickSpeed * deltaSeconds * .5f);
				}
			}
		}

		//Fire bullets
		if (m_XboxController.getRightTriggerValue() > 0.f) {
			if (m_rapidFireAge > RAPID_FIRE_TIME) {
				m_rapidFireAge = 0.f;
				fireBullet(&g_theWorld->m_playerTank);
			}
		}

		//DASH LEFT
		if (m_XboxController.getButtonDown(XB_LB)) {
			g_theWorld->m_playerTank.dash(DASH_LEFT);
		}
		//DASH RIGHT
		if (m_XboxController.getButtonDown(XB_RB)) {
			g_theWorld->m_playerTank.dash(DASH_RIGHT);
		}
	}

	//Different modes
	if (m_XboxController.getButtonDown(XB_START)) {
		if (g_theUI->inAttractMode()) {
			g_theWorld->m_playerTank.setHealth(g_theWorld->m_playerTank.getMaxHealth());
			g_theUI->leaveAttractMode();
		}
		else if (g_theUI->inWinScreen()) {
			g_theUI->disableWinScreen();
			g_theUI->goToAttractMode();
		}
		else if (g_theUI->isPaused()) {
			if (!g_theWorld->m_playerTank.isAlive()) {
				revivePlayer();
			}
			g_theUI->unpauseGame();
		}
		else {
			if (g_theWorld->m_playerTank.isAlive())
				g_theUI->pauseGame();
		}
	}

	if (m_XboxController.getButtonDown(XB_BACK)) {
		if (g_theUI->inAttractMode())
			g_theApp->quitGame();
		else if (g_theUI->inWinScreen()) {
			g_theUI->disableWinScreen();
			g_theUI->goToAttractMode();
		}
		else if (!g_theUI->isPaused())
			g_theUI->pauseGame();
		else if (g_theUI->isPaused())
			g_theUI->goToAttractMode();
	}
}

//----------------------------------COLLISION CHECKS--------------------------------------
void TheGame::checkTileCollision(Entity* entity) {
	TileCoords playerTC = g_theWorld->m_gameMap.getTileCoordsForWorldCoords(entity->getPosition());

	//-------------------------North tile------------------------------------------------------------
	Tile* north = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(0, 1));

	if (north->isSolid()) {
		if (entity->getPosition().y + entity->getPhysicsRadius() > (float)north->getTileCoords().y) {
			float toMove = (float)north->getTileCoords().y - (entity->getPosition().y + entity->getPhysicsRadius());
			entity->setPosition(Vector2(entity->getPosition().x, entity->getPosition().y + toMove));
		}
	}

	//-------------------------South tile------------------------------------------------------------
	Tile* south = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(0, -1));

	if (south->isSolid()) {
		if (entity->getPosition().y - entity->getPhysicsRadius() < (float)south->getTileCoords().y + 1) {
			float toMove = (float)south->getTileCoords().y + 1.f - (entity->getPosition().y - entity->getPhysicsRadius());
			entity->setPosition(Vector2(entity->getPosition().x, entity->getPosition().y + toMove));
		}
	}

	//-------------------------East tile------------------------------------------------------------
	Tile* east = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(1, 0));

	if (east->isSolid()) {
		if (entity->getPosition().x + entity->getPhysicsRadius() > (float)east->getTileCoords().x) {
			float toMove = (float)east->getTileCoords().x - (entity->getPosition().x + entity->getPhysicsRadius());
			entity->setPosition(Vector2(entity->getPosition().x + toMove, entity->getPosition().y));
		}
	}

	//-------------------------West tile------------------------------------------------------------
	Tile* west = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(-1, 0));

	if (west->isSolid()) {
		if (entity->getPosition().x - entity->getPhysicsRadius() < (float)west->getTileCoords().x + 1) {
			float toMove = (float)west->getTileCoords().x + 1.f - (entity->getPosition().x - entity->getPhysicsRadius());
			entity->setPosition(Vector2(entity->getPosition().x + toMove, entity->getPosition().y));
		}
	}

	//-------------------------NE tile------------------------------------------------------------
	Tile* north_east = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(1, 1));
	Vector2 tile_position = Vector2((float)north_east->getTileCoords().x, (float)north_east->getTileCoords().y);

	if (north_east->isSolid()) {
		if (IsPointInDisk(tile_position, entity->getPosition(), entity->getPhysicsRadius())) {
			pushEntityFromPoint(entity, tile_position);
		}
	}

	//-------------------------SE tile------------------------------------------------------------
	Tile* south_east = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(1, -1));
	tile_position = Vector2((float)south_east->getTileCoords().x, (float)south_east->getTileCoords().y + 1.f);

	if (south_east->isSolid()) {
		if (IsPointInDisk(tile_position, entity->getPosition(), entity->getPhysicsRadius())) {
			pushEntityFromPoint(entity, tile_position);
		}
	}

	//-------------------------NW tile------------------------------------------------------------
	Tile* north_west = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(-1, 1));
	tile_position = Vector2((float)north_west->getTileCoords().x + 1.f, (float)north_west->getTileCoords().y);

	if (north_west->isSolid()) {
		if (IsPointInDisk(tile_position, entity->getPosition(), entity->getPhysicsRadius())) {
			pushEntityFromPoint(entity, tile_position);
		}
	}

	//-------------------------SW tile------------------------------------------------------------
	Tile* south_west = g_theWorld->m_gameMap.getTileAtTileCoords(playerTC + IntVector2(-1, -1));
	tile_position = Vector2((float)south_west->getTileCoords().x + 1.f, (float)south_west->getTileCoords().y + 1.f);

	if (south_west->isSolid()) {
		if (IsPointInDisk(tile_position, entity->getPosition(), entity->getPhysicsRadius())) {
			pushEntityFromPoint(entity, tile_position);
		}
	}
}
void TheGame::checkEntityCollision(Entity* entity) {

	//Check entity against the player
	PlayerTank* player = &g_theWorld->m_playerTank;

	//Check PlayerTank vs EnemyTank
	if (DoDiscsOverlap(player->getPosition(), player->getPhysicsRadius(), entity->getPosition(), entity->getPhysicsRadius()) && entity->getType() != ENTITY_BULLET) {
		Vector2 pushDir = entity->getPosition() - player->getPosition();
		pushDir.normalize();
		float dist = DistanceBetweenPoints(player->getPosition(), entity->getPosition());
		pushDir = pushDir * (dist - entity->getPhysicsRadius() - player->getPhysicsRadius());

		if (entity->getType() != ENTITY_TURRET)
			entity->setPosition(entity->getPosition() - pushDir);
		player->setPosition(player->getPosition() + pushDir);
	}

	//Check entity against other entities
	std::set<Entity*>::iterator it;
	if (!g_theWorld->m_entities.empty()) {
		for (it = g_theWorld->m_entities.begin(); it != g_theWorld->m_entities.end(); ++it) {
			Entity* otherEntity = *it;

			if (entity->getType() != ENTITY_BULLET && otherEntity->getType() != ENTITY_BULLET && otherEntity != entity) {
				//Check PlayerTank vs EnemyTank
				if (DoDiscsOverlap(entity->getPosition(), entity->getPhysicsRadius(), otherEntity->getPosition(), otherEntity->getPhysicsRadius())) {
					Vector2 pushDir = otherEntity->getPosition() - entity->getPosition();
					pushDir.normalize();
					float dist = DistanceBetweenPoints(otherEntity->getPosition(), entity->getPosition());
					pushDir = pushDir * (dist - otherEntity->getPhysicsRadius() - entity->getPhysicsRadius());

					if (entity->getType() != ENTITY_TURRET)
						entity->setPosition(entity->getPosition() + pushDir);
					if (otherEntity->getType() != ENTITY_TURRET)
						otherEntity->setPosition(otherEntity->getPosition() - pushDir);
				}
			}
		}
	}
}
void TheGame::checkBulletCollision() {
	std::set<Entity*>::iterator it;

	if (!g_theWorld->m_entities.empty()) {
		for (it = g_theWorld->m_entities.begin(); it != g_theWorld->m_entities.end(); ++it) {
			Entity* entity = *it;

			if (entity->getType() == ENTITY_BULLET) {
				
				//Check to see if bullet is hitting tile
				if (g_theWorld->m_gameMap.getTileAtWorldCoords(entity->getPosition())->isSolid()) {
					g_theWorld->addSpriteAnim(entity->getPosition(), ANIM_SIZE_BULLET, 1.f);
					g_theWorld->m_entities.erase(it);
					delete entity;
					return;
				}

				//Check to see if enemy bullet is hitting player
				Bullet* bullet = dynamic_cast<Bullet*>(entity);
				if (!bullet->belongsToPlayer()) {
					if (DoDiscsOverlap(entity->getPosition(), ENTITY_SIZE, g_theWorld->m_playerTank.getPosition(), g_theWorld->m_playerTank.getPhysicsRadius())) {
						g_theWorld->m_playerTank.decrementHealth();

						if (!g_theWorld->m_playerTank.isAlive())
							killPlayer();
						g_theWorld->m_entities.erase(it);

						g_theWorld->addSpriteAnim(entity->getPosition(), ANIM_SIZE_BULLET, 1.f);
						g_theWorld->PlaySound(EFFECT_BULLET_HIT_ENEMY);

						delete entity;
						return;
					}
				}

				//Loop through enemies and check if bullet is colliding with any of them
				std::set<Entity*>::iterator loop;
				for (loop = g_theWorld->m_entities.begin(); loop != g_theWorld->m_entities.end(); ++loop) {
					Entity* otherEntity = *loop;

					if (otherEntity->getType() != ENTITY_BULLET) {
						if (DoDiscsOverlap(entity->getPosition(), ENTITY_SIZE, otherEntity->getPosition(), ENTITY_SIZE)) {
							if (dynamic_cast<Bullet*>(entity)->belongsToPlayer()) {
								g_theWorld->m_entities.erase(it);

								if (otherEntity->isAlive()) {
									otherEntity->decrementHealth();
									g_theWorld->addSpriteAnim(entity->getPosition(), ANIM_SIZE_BULLET, 1.f);
									g_theWorld->PlaySound(EFFECT_BULLET_HIT_ENEMY);
								}
								else {
									g_theWorld->PlaySound(EFFECT_ENEMY_DEATH);
									g_theWorld->addSpriteAnim(otherEntity->getPosition(), 1.2f, 3.f);
									g_theWorld->m_entities.erase(loop);
									delete otherEntity;
								}
								delete entity;
								return;
							}
						}
					}
				}
			}
		}
	}
}

void TheGame::pushEntityFromPoint(Entity* entity, Vector2 position) {
	Vector2 pushDir = entity->getPosition() - position;
	pushDir.normalize();
	float dist = DistanceBetweenPoints(position, entity->getPosition());
	pushDir = pushDir * (entity->getPhysicsRadius() - dist);
	entity->setPosition(entity->getPosition() + pushDir);
}

//----------------------------------OTHER--------------------------------------
void TheGame::fireBullet(Entity* entity) {
	g_theWorld->PlaySound(EFFECT_PLAYER_SHOOT);

	Vector2 entityTip = entity->getPosition() + Vector2(entity->getPhysicsRadius() * 1.5f * cos(entity->getOrientation()), entity->getPhysicsRadius() * 1.5f * sin(entity->getOrientation()));
	Bullet* b = new Bullet( entityTip, Vector2(BULLET_SPEED, BULLET_SPEED), Vector2(0.f, 0.f), entity->getOrientation(), PLAYER_BULLET_SIZE, true);
	g_theWorld->m_entities.insert(b);

}

void TheGame::spawnEnemies() {

	int chanceToSpawn = 0;
	int whichEnemyToSpawn = 0;
	for (int x = 0; x < MAP_SIZE.x; x++) {
		for (int y = 0; y < MAP_SIZE.y; y++) {

			Tile* t = g_theWorld->m_gameMap.getTileAtTileCoords(TileCoords(x, y));
			chanceToSpawn = rand() % 100;

			if (chanceToSpawn > 90 && !t->isSolid() && y > 4) {
				whichEnemyToSpawn = rand() % 2;
				float orientation = Randf(-2.f * pi, 2.f * pi);

				if (whichEnemyToSpawn == 1)
					g_theWorld->m_entities.insert(new EnemyTank(t->getCenterInWorldCoords(), orientation));
				else
					g_theWorld->m_entities.insert(new EnemyTurret(t->getCenterInWorldCoords(), orientation));
			}
		}
	}
}

void TheGame::killPlayer() {
	if (!m_playerKilled) {
		m_playerKilled = true;
		g_theWorld->addSpriteAnim(g_theWorld->m_playerTank.getPosition(), 1.2f, 3.f);
		g_theWorld->StopMusic();
		g_theWorld->PlaySound(EFFECT_PLAYER_DEATH);
		g_theUI->killPlayer();
	}
}
void TheGame::revivePlayer() {
	g_theWorld->m_playerTank.setHealth(g_theWorld->m_playerTank.getMaxHealth());
	g_theUI->revivePlayer();
	g_theWorld->StopMusic();
	g_theWorld->PlayMusic(MUSIC_TRACK_1);
	m_playerKilled = false;
}