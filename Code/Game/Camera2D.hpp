#pragma once

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/StringUtils.hpp"

class Camera2D;
extern Camera2D* g_theCamera;

class Camera2D {
public:
	//STRUCTORS
	Camera2D();
	Camera2D(const Vector2& screenSize, const Vector2& pos, const Vector2& ViewPort, float orientation);
	~Camera2D();

	//TRANSLATIONS
	void MoveAndRotateCamera(const Vector2& pos, float orientation);
	void MoveCamera(Vector2 position);
	void RotateCamera(float orientation);
	

	//ACCESSORS
	Vector2 GetCameraPosition() const;
	Vector2 GetViewPort() const;
	float GetCameraRotation() const;
	Vector2 ScreenSize() const;
private:
	Vector2 m_screenSize;
	Vector2 m_cameraPos;
	Vector2 m_viewPort; //cameraCenteredOnScreenPosition
	float m_orientation;
};