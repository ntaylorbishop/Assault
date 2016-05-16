#pragma once

#define STBI_HEADER_FILE_ONLY
#include "ThirdParty/stb_image.c"

#include "Engine/Renderer/Spritesheet.hpp"
#include "Engine/Renderer/SpriteAnim.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/ErrorWarningAssert.hpp"
#include "Engine/Audio.hpp"
#include "Game/Map.hpp"
#include "Game/Entity.hpp"
#include "Game/EnemyTank.hpp"
#include "Game/EnemyTurret.hpp"
#include "Game/PlayerTank.hpp"
#include "Game/Camera2D.hpp"

using namespace std;

class TheWorld;
extern TheWorld* g_theWorld;

typedef void* AudioChannelHandle;

enum SOUND_EFFECT {
	EFFECT_BULLET_HIT_ENEMY = 0,
	EFFECT_PLAYER_DEATH,
	EFFECT_ENEMY_TANK_SHOOT,
	EFFECT_ENEMY_TURRET_SHOOT,
	EFFECT_PLAYER_SHOOT,
	EFFECT_PAUSE_GAME,
	EFFECT_UNPAUSE_GAME, 
	EFFECT_ENEMY_DEATH
};

enum SOUND_MUSIC {
	MUSIC_TRACK_1 = 0,
	MUSIC_START_SCREEN
};

struct SpriteAnimWithPos {
	SpriteAnim anim;
	Vector2 pos;
	float size;
};

class TheWorld {
public:
	//STRUCTORS
	TheWorld(const IntVector2& worldSize, const Vector2& screenSize, const Vector2& camera_viewPort, float entitySize);
	~TheWorld();
	
	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render();

	//SOUNDS
	void PlaySound(SOUND_EFFECT effect);
	void PlayMusic(SOUND_MUSIC music);
	void PauseMusic();
	void UnpauseMusic();
	void StopMusic();

	//SPRITE ANIMS
	void addSpriteAnim(Vector2 pos, float size, float duration);

	//MAP LOADING
	void loadMap(int level);

public:
	set<Entity*> m_entities;
	Map m_gameMap;
	PlayerTank m_playerTank;
	Camera2D m_mainCamera;
	SpriteSheet* m_tileSpritesheet;

	vector<SpriteAnimWithPos*> m_spriteAnims;

private:
	char* level1Path = "Data/Maps/Level1.png";
	IntVector2 m_mapSize;

	char* m_tileSpritesheetPath = "Data/Images/Tile_Spritesheet.png";

	AudioSystem m_audioSystem;

	SoundID m_audio_StartMusic;
	SoundID m_audio_MusicTrack1;
	SoundID m_audio_BulletHitEnemy;
	SoundID m_audio_PlayerDeath;
	SoundID m_audio_EnemyTankShoot;
	SoundID m_audio_EnemyTurretShoot;
	SoundID m_audio_PlayerShoot;
	SoundID m_audio_PauseGame;
	SoundID m_audio_UnpauseGame;
	SoundID m_audio_EnemyDeath;

	AudioChannelHandle m_audio_StartMusicChannel;
	AudioChannelHandle m_audio_MusicTrack1Channel;

	char* m_explosionTexturePath;
	SpriteSheet* m_explosionSpriteSheet;
};