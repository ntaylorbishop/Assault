#include "Game/TheWorld.hpp"

TheWorld* g_theWorld = nullptr;

//STRUCTORS----------------------------------------------------------------------------------
TheWorld::TheWorld(const IntVector2& worldSize, const Vector2& screenSize, const Vector2& camera_viewPort, float entitySize)
    : m_gameMap(worldSize)
    , m_playerTank()
	, m_tileSpritesheet(nullptr)
{
	Vector2 position = Vector2(0.f, 0.f);

	m_gameMap = Map(worldSize);

	m_mainCamera = Camera2D(screenSize, position, camera_viewPort, 2.f * pi);
	m_playerTank = PlayerTank(m_mainCamera.GetViewPort(), pi / 2.f, entitySize);
	m_tileSpritesheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(2, 1), "Data/Images/Tile_Spritesheet.png");

	//Audio paths
	m_audio_StartMusic = m_audioSystem.CreateOrGetSound("Data/Sounds/StartMusic.mp3");
	m_audio_MusicTrack1 = m_audioSystem.CreateOrGetSound("Data/Sounds/MusicTrack1.mp3");
	m_audio_BulletHitEnemy = m_audioSystem.CreateOrGetSound("Data/Sounds/BulletHitEnemy.wav");
	m_audio_PlayerDeath = m_audioSystem.CreateOrGetSound("Data/Sounds/PlayerDeath.wav");
	m_audio_EnemyTankShoot = m_audioSystem.CreateOrGetSound("Data/Sounds/EnemyTankShoot.wav");
	m_audio_EnemyTurretShoot = m_audioSystem.CreateOrGetSound("Data/Sounds/EnemyTurretShoot.wav");
	m_audio_PlayerShoot = m_audioSystem.CreateOrGetSound("Data/Sounds/PlayerShoot.wav");
	m_audio_PauseGame = m_audioSystem.CreateOrGetSound("Data/Sounds/Pause.wav");
	m_audio_UnpauseGame = m_audioSystem.CreateOrGetSound("Data/Sounds/Unpause.wav");
	m_audio_EnemyDeath = m_audioSystem.CreateOrGetSound("Data/Sounds/EnemyDeath.wav");

	m_explosionTexturePath = "Data/Images/Explosion_5x5.png";
	m_explosionSpriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(5, 5), m_explosionTexturePath);

	loadMap(1);
}
TheWorld::~TheWorld() { 
	std::set<Entity*>::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it) {
		delete *it;
	}
}

//UPDATE RENDER----------------------------------------------------------------------------------
void TheWorld::Update(float deltaSeconds) {
	m_audioSystem.Update(deltaSeconds);

	for (unsigned int i = 0; i < m_spriteAnims.size(); i++)
		m_spriteAnims[i]->anim.Update(deltaSeconds);


	//Clean sprite animations
	for (std::vector<SpriteAnimWithPos*>::iterator it = m_spriteAnims.begin(); it != m_spriteAnims.end(); ++it) {
		if ((*it)->anim.isFinished()) {
			m_spriteAnims.erase(it);
			return;
		}
	}
}
void TheWorld::Render() {
	m_gameMap.Render();

	//Render entities
	std::set<Entity*>::iterator it;
	for (it = m_entities.begin(); it != m_entities.end(); ++it) {
		Entity* entity = *it;
		entity->Render();
	}

	//Render sprite anims
	for (unsigned int i = 0; i < m_spriteAnims.size(); i++) {
		AABB2 quad = AABB2(m_spriteAnims[i]->pos - m_spriteAnims[i]->size, m_spriteAnims[i]->pos + m_spriteAnims[i]->size);

		g_theRenderer->BlendMode(GL_SRC_ALPHA, GL_ONE);
		g_theRenderer->DrawTexturedAABB2FromSpriteSheet(m_spriteAnims[i]->anim.getSpriteSheet(), RGBA(1.f, 1.f, 1.f, 1.f), quad, m_spriteAnims[i]->anim.getCurrentTexCoords());
		g_theRenderer->BlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	g_theRenderer->LoadIdentity();
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	if (m_playerTank.isAlive())
		m_playerTank.Render();
}

//SOUNDS-----------------------------------------------------------------------------------------
void TheWorld::PlaySound(SOUND_EFFECT effect) {
	if (effect == EFFECT_BULLET_HIT_ENEMY)
		m_audioSystem.Play(m_audio_BulletHitEnemy);
	else if (effect == EFFECT_ENEMY_TANK_SHOOT)
		m_audioSystem.Play(m_audio_EnemyTankShoot);
	else if (effect == EFFECT_ENEMY_TURRET_SHOOT)
		m_audioSystem.Play(m_audio_EnemyTurretShoot);
	else if (effect == EFFECT_PLAYER_DEATH)
		m_audioSystem.Play(m_audio_PlayerDeath);
	else if (effect == EFFECT_PLAYER_SHOOT)
		m_audioSystem.Play(m_audio_PlayerShoot);
	else if (effect == EFFECT_PAUSE_GAME)
		m_audioSystem.Play(m_audio_PauseGame);
	else if (effect == EFFECT_UNPAUSE_GAME)
		m_audioSystem.Play(m_audio_UnpauseGame);
	else if (effect == EFFECT_ENEMY_DEATH)
		m_audioSystem.Play(m_audio_EnemyDeath);
	else
		DebuggerPrintf("Tried to play an incorrect sound in %s on line # %i", __FILE__, __LINE__);
}
void TheWorld::PlayMusic(SOUND_MUSIC music) {
	if (music == MUSIC_TRACK_1)
		m_audio_MusicTrack1Channel = m_audioSystem.PlayLoop(m_audio_MusicTrack1);
	else if (music == MUSIC_START_SCREEN)
		m_audio_StartMusicChannel = m_audioSystem.PlayLoop(m_audio_StartMusic);
	else
		DebuggerPrintf("Tried to play an incorrect sound in %s on line # %i", __FILE__, __LINE__);
}
void TheWorld::PauseMusic() {
	m_audioSystem.PauseChannel(m_audio_MusicTrack1Channel);
	m_audioSystem.PauseChannel(m_audio_StartMusicChannel);
}
void TheWorld::UnpauseMusic() {
	m_audioSystem.UnpauseChannel(m_audio_MusicTrack1Channel);
	m_audioSystem.UnpauseChannel(m_audio_StartMusicChannel);
}
void TheWorld::StopMusic() {
	m_audioSystem.StopChannel(m_audio_MusicTrack1Channel);
	m_audioSystem.StopChannel(m_audio_StartMusicChannel);
}

//SPRITE ANIMS----------------------------------------------------------------------------------
void TheWorld::addSpriteAnim(Vector2 pos, float size, float duration) {
	SpriteAnimWithPos* animWithPos = new SpriteAnimWithPos;
	animWithPos->anim = SpriteAnim(m_explosionSpriteSheet, duration, ANIM_MODE_PLAY_TO_END, 0, 23);
	animWithPos->pos = pos;
	animWithPos->size = size;

	m_spriteAnims.push_back(animWithPos);
}

//MAP LOADING-----------------------------------------------------------------------------------
void TheWorld::loadMap(int level) {
	int numComponents = 0;
	int numComponentsRequested = 3;

	m_gameMap.clearMap();
	m_entities.clear();


	if (level == 1) {
		unsigned char* imageData = stbi_load(level1Path, &m_mapSize.x, &m_mapSize.y, &numComponents, numComponentsRequested);
		m_gameMap.setSize(m_mapSize);

		int i = 0;
		for (int y = 0; y < m_mapSize.y; y++) {
			for (int x = m_mapSize.x; x > 0; x--) {
				unsigned char r = imageData[i];
				i++;
				unsigned char g = imageData[i];
				i++;
				unsigned char b = imageData[i];
				i++;

				if (r == 155 && g == 155 && b == 155)
					m_gameMap.pushTile(IntVector2(x, y), TILE_STONE);
				else if (r == 255 && g == 255 && b == 255) {
					m_gameMap.pushTile(IntVector2(x, y), TILE_GRASS);
					DebuggerPrintf("Player pos: (%f, %f)\n", m_playerTank.getPosition().x, m_playerTank.getPosition().y);
					m_playerTank.setPosition(Vector2((float)x, (float)y) + Vector2(0.5f, 0.5f));
					DebuggerPrintf("Player pos: (%f, %f)\n", m_playerTank.getPosition().x, m_playerTank.getPosition().y);
					m_playerTank.setOrientation(-pi / 2.f);
				}
				else if (r == 0 && g == 0 && b == 0)
					m_gameMap.pushTile(IntVector2(x, y), TILE_DESERT);
				else if (r == 255 && g == 0 && b == 0) {
					m_gameMap.pushTile(IntVector2(x, y), TILE_GRASS);

					float orientation = Randf(-2.f * pi, 2.f * pi);
					m_entities.insert(new EnemyTank(Vector2((float)x, (float)y) + Vector2(0.5f, 0.5f), orientation));
				}
				else if (r == 255 && g == 0 && b == 255) {
					m_gameMap.pushTile(IntVector2(x, y), TILE_GRASS);

					float orientation = Randf(-2.f * pi, 2.f * pi);
					m_entities.insert(new EnemyTurret(Vector2((float)x, (float)y) + Vector2(0.5f, 0.5f), orientation));
				}
				else
					m_gameMap.pushTile(IntVector2(x, y), TILE_GRASS);
			}
		}
	}
}