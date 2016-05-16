#include "Game/Camera2D.hpp"

//----------------------------------STRUCTORS--------------------------------------
Camera2D::Camera2D() {}
Camera2D::Camera2D(const Vector2& screenSize, const Vector2& pos, const Vector2& ViewPort, float orientation) {
	m_screenSize = screenSize;
	m_cameraPos = pos;
	m_viewPort = ViewPort;
	m_orientation = orientation;
}
Camera2D::~Camera2D() {

}

//----------------------------------TRANSLATIONS--------------------------------------
void Camera2D::MoveAndRotateCamera(const Vector2& pos, float orientation) {
	m_cameraPos = pos;
	m_orientation = orientation;
}
void Camera2D::MoveCamera(Vector2 position) {
	m_cameraPos = position;
}
void Camera2D::RotateCamera(float orientation) {
	m_orientation = orientation + (3.f / 2.f) * pi;
	//DebuggerPrintf();
}

//----------------------------------ACCESSORS--------------------------------------
Vector2 Camera2D::GetCameraPosition() const {
	return m_cameraPos;
}
Vector2 Camera2D::GetViewPort() const {
	return Vector2(m_screenSize.x * m_viewPort.x, m_screenSize.y * m_viewPort.y);
}
float Camera2D::GetCameraRotation() const {
	return m_orientation;
}
Vector2 Camera2D::ScreenSize() const {
	return m_screenSize;
}