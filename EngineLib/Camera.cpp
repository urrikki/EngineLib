#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}



void Camera::getViewMatrix(Mat& view)
{

	m_view = m_world;
	m_view.inverse();
	view = m_view;
}

void Camera::getProjectionMatrix(Mat& proj)
{
	proj = m_projection;
}


void Camera::setFarPlane(float farPlane)
{
	m_farPlane = farPlane;
	m_projection.setPerspectiveFovLH(1.3f, (float)m_screenArea.width / (float)m_screenArea.height, 0.1f, m_farPlane);
}

void Camera::setScreenArea(const  Rect& screen)
{
	m_screenArea = screen;
	m_projection.setPerspectiveFovLH(1.3f, (float)m_screenArea.width / (float)m_screenArea.height, 0.1f, m_farPlane);
}