#pragma once

#include "framework.h"
#include "Input.h"
#include "ShapeApp.h"
#include "Math.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void update(Input& input);

	void getViewMatrix(Mat& view);
	void getProjectionMatrix(Mat& proj);

	void setFarPlane(float farPlane);
	void setScreenArea(const  Rect& screen);

private:
	Mat m_view;
	Mat m_projection;
	Mat m_world;

	float m_farPlane = 100.0f;
	Rect m_screenArea;

};
