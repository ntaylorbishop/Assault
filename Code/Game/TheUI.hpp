#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/TheWorld.hpp"

class TheUI;
extern TheUI* g_theUI;

const RGBA PAUSE_TINT = RGBA(0.f, 0.f, 0.f, 0.3f);

class TheUI {
public:
	TheUI();
	TheUI(Vector2 screenSize);
	~TheUI();

	void Update(float deltaSeconds);
	void Render();

	void renderHUD();

	//Pause Screen
	void renderPauseScreen();
	void pauseGame();
	void unpauseGame();
	bool isPaused();

	//Start Screen
	void renderStartMenu();
	void goToAttractMode();
	void leaveAttractMode();
	bool inAttractMode();

	//Death Screen
	void renderDeathScreen();
	void killPlayer();
	void revivePlayer();

	//Win screen
	void renderWinScreen();
	void showWinScreen();
	void disableWinScreen();
	bool inWinScreen();
	float winScreenAge();
private:
	Vector2 m_screenSize;

	char* m_pauseTextPath = "Data/Images/PauseScreen.png";
	bool m_isGamePaused;
	Texture* m_pauseTextImg;
	Vector2 m_pauseTextImgSize;
	float m_pauseAge;

	char* m_attractTextPath = "Data/Images/AttractScreen.png";
	Texture* m_startScreenImg;
	bool m_inAttractScreen;

	char* m_deathImgPath = "Data/Images/DeathScreen.png";
	Texture* m_deathImg;
	bool m_inDeathScreen;
	float m_deathAge;

	char* m_winScreenPath = "Data/Images/WinScreen.png";
	Texture* m_winScreen;
	bool m_inWinScreen;
	float m_winAge;
	
};