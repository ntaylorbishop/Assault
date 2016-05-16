#pragma once

#include <string>

class TheApp;
extern TheApp* g_theApp;

using namespace std;

const float DOUBLE_TAP_TOLERANCE = .5f;

class TheApp {
public:
	//STRUCTORS
	TheApp();

	void Update(float deltaSeconds);

	void setKeyDownStatus(unsigned char asKey, bool isNowDown);

	bool getKey(unsigned char asKey);
	bool getKeyDown(unsigned char asKey);

	bool isQuitting();
	void quitGame();

private:
	bool m_isKeyDown[256];
	bool m_isQuitting;
};