#include "Game/TheApp.hpp"

TheApp* g_theApp = nullptr;

TheApp::TheApp() {
	for (int i = 0; i < 256; i++)
		m_isKeyDown[i] = false;
	m_isQuitting = false;
}

void TheApp::Update(float deltaSeconds) {
	deltaSeconds = deltaSeconds;
}

void TheApp::setKeyDownStatus(unsigned char asKey, bool isNowDown) {
	m_isKeyDown[asKey] = isNowDown;
}

bool TheApp::getKey(unsigned char asKey) {
	return m_isKeyDown[asKey];
}
bool TheApp::getKeyDown(unsigned char asKey) {
	bool b = m_isKeyDown[asKey];
	m_isKeyDown[asKey] = false;

	return b;
}

bool TheApp::isQuitting() {
	return m_isQuitting;
}
void TheApp::quitGame() {
	m_isQuitting = true;
}