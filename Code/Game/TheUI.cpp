#include "Game/TheUI.hpp"

TheUI* g_theUI = nullptr;

//STRUCTORS------------------------------------------------------------------
TheUI::TheUI()
    : m_screenSize()
    , m_pauseTextImg(nullptr)
	, m_isGamePaused(true)
	, m_pauseTextImgSize()
	, m_startScreenImg(nullptr)
	, m_pauseAge(0.f)
	, m_inAttractScreen(true)
	, m_deathImg(nullptr)
	, m_deathAge(0.f)
	, m_inDeathScreen(false)
	, m_inWinScreen(false)
	, m_winAge(0.f)
{}
TheUI::TheUI(Vector2 screenSize) 
    : m_screenSize()
    , m_pauseTextImg(nullptr)
	, m_isGamePaused(true)
	, m_pauseTextImgSize()
	, m_startScreenImg(nullptr)
	, m_pauseAge(0.f)
	, m_inAttractScreen(true)
	, m_deathImg(nullptr)
	, m_deathAge(0.f)
	, m_inDeathScreen(false)
	, m_inWinScreen(false)
	, m_winAge(0.f)
{
	m_screenSize = screenSize;
	m_pauseTextImg = Texture::CreateOrGetTexture(m_pauseTextPath);
	m_pauseTextImgSize = Vector2(screenSize.x * .2f, screenSize.y * .05f);

	m_startScreenImg = Texture::CreateOrGetTexture(m_attractTextPath);
	m_pauseTextImg = Texture::CreateOrGetTexture(m_pauseTextPath);
	m_deathImg = Texture::CreateOrGetTexture(m_deathImgPath);
	m_winScreen = Texture::CreateOrGetTexture(m_winScreenPath);
}
TheUI::~TheUI() {}

//UPDATE RENDER-------------------------------------------------------------
void TheUI::Update(float deltaSeconds) {
	m_pauseAge += deltaSeconds;
	if (m_inDeathScreen)
		m_deathAge += deltaSeconds;
	else
		m_deathAge -= deltaSeconds;

	if (m_inWinScreen)
		m_winAge += deltaSeconds;

	DebuggerPrintf("%f\n", m_winAge);
}
void TheUI::Render() {
	if (m_isGamePaused) {
		renderPauseScreen();
		renderHUD();
	}
	if (m_isGamePaused && m_inAttractScreen) {
		renderStartMenu();
	}
	else if (!m_isGamePaused)
		renderHUD();

	renderDeathScreen();
	if (m_inWinScreen)
		renderWinScreen();
}

void TheUI::renderHUD() {
	int playerHealth = g_theWorld->m_playerTank.getHealth();

	Vector2 healthStartPos = Vector2(m_screenSize.x - 3.f, m_screenSize.y - 0.5f);

	for (int i = 0; i < playerHealth; i++) {
		AABB2 healthBar = AABB2(healthStartPos + Vector2(0.f, 0.f), healthStartPos + Vector2(0.2f, 0.3f));
		g_theRenderer->DrawAABB2(healthBar, RGBA(1.f, 1.f, 1.f, 1.f), PRIMITIVE_QUADS);
		healthStartPos = healthStartPos + Vector2(0.2f, 0.f);
	}
}

//PAUSE GAME--------------------------------------------------------------------------------
void TheUI::renderPauseScreen() {

	g_theRenderer->LoadIdentity();
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	AABB2 quad = AABB2(Vector2(0.f, 0.f), Vector2(m_screenSize));

	if (g_theWorld->m_playerTank.isAlive())
		g_theRenderer->DrawTexturedAABB2(m_pauseTextImg, RGBA(1.f, 1.f, 1.f, 1.f), quad);
	else
		g_theRenderer->DrawTexturedAABB2(m_deathImg, RGBA(1.f, 1.f, 1.f, 1.f), quad);
}
void TheUI::pauseGame() {
	g_theWorld->PauseMusic();
	g_theWorld->PlaySound(EFFECT_PAUSE_GAME);
	m_isGamePaused = true;
}
void TheUI::unpauseGame() {
	g_theWorld->UnpauseMusic();
	g_theWorld->PlaySound(EFFECT_UNPAUSE_GAME);
	m_isGamePaused = false;
}
bool TheUI::isPaused() {
	return m_isGamePaused;
}

//ATTRACT MODE---------------------------------------------------------------------------------
void TheUI::renderStartMenu() {
	g_theRenderer->LoadIdentity();
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	AABB2 startScreenQuad = AABB2(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	g_theRenderer->DrawTexturedAABB2(m_startScreenImg, RGBA(1.f, 1.f, 1.f, 1.f), startScreenQuad);
}
void TheUI::goToAttractMode() {
	m_inDeathScreen = false;
	m_isGamePaused = true;
	m_inAttractScreen = true;
	m_inWinScreen = false;
	g_theWorld->StopMusic();
	g_theWorld->PlayMusic(MUSIC_START_SCREEN);
}
void TheUI::leaveAttractMode() {
	m_inDeathScreen = false;
	m_isGamePaused = false;
	m_inAttractScreen = false;
	m_inWinScreen = false;
	g_theWorld->StopMusic();
	g_theWorld->PlayMusic(MUSIC_TRACK_1);

	g_theWorld->loadMap(1);
}
bool TheUI::inAttractMode() {
	return m_inAttractScreen;
}

//DEATH SCREEN------------------------------------------------------------------------------
void TheUI::renderDeathScreen() {
	g_theRenderer->LoadIdentity();
	g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));
	AABB2 startScreenQuad = AABB2(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

	g_theRenderer->DrawTexturedAABB2(m_deathImg, RGBA(1.f, 1.f, 1.f, m_deathAge / 3.f), startScreenQuad);

	if (m_deathAge <= 0.f)
		m_inDeathScreen = false;
	if (m_deathAge > 10.f) {
		m_deathAge = 0.f;
		goToAttractMode();
	}
}
void TheUI::killPlayer() {
	m_inDeathScreen = true;
	m_deathAge = 0.f;
}
void TheUI::revivePlayer() {
	//m_isGamePaused = false;
	m_inDeathScreen = false;
	m_deathAge = 1.f;

	g_theWorld->loadMap(1);
}

//WIN SCREEN----------------------------------------------------------------------------------
void TheUI::renderWinScreen() {
	if (m_inWinScreen) {
		g_theRenderer->LoadIdentity();
		g_theRenderer->SetOrtho2D(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

		AABB2 quad = AABB2(Vector2(0.f, 0.f), Vector2(16.f, 9.f));

		g_theRenderer->DrawTexturedAABB2(m_winScreen, RGBA(1.f, 1.f, 1.f, 1.f), quad);

		if (m_winAge > 10.f)
			goToAttractMode();
	}
}
void TheUI::showWinScreen() {
	m_inWinScreen = true;
}
void TheUI::disableWinScreen() {
	m_winAge = 0.f;
	m_inWinScreen = false;
}
bool TheUI::inWinScreen() {
	return m_inWinScreen;
}
float TheUI::winScreenAge() {
	return m_winAge + 1.f;
}