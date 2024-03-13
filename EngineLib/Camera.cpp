#include "pch.h"
#include "Camera.h"

Camera::Camera() :
    mPosition(0.0f, 0.0f, 0.0f),
    mRight(1.0f, 0.0f, 0.0f),
    mUp(0.0f, 1.0f, 0.0f),
    mLook(0.0f, 0.0f, 1.0f),
    mFOV(XM_PIDIV4),
    mAspect(16.0f / 9.0f),
    mNearPlane(0.1f),
    mFarPlane(1000.0f),
    mMouseSensitivity(0.1f)
{
    UpdateViewMatrix();
    mProjectionMatrix = XMMatrixPerspectiveFovLH(mFOV, mAspect, mNearPlane, mFarPlane);
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
    POINT mouseDelta = Input::getMouseDelta();

    mPosition.x += mLook.x * mouseDelta.x * mMouseSensitivity;
    mPosition.y += mLook.y * mouseDelta.y * mMouseSensitivity;
    mPosition.z += mLook.z * mouseDelta.y * mMouseSensitivity;

    UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    mPosition = XMFLOAT3(x, y, z);
    UpdateViewMatrix();
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
    // Not implemented for this simple camera
}

void Camera::Pitch(float angle)
{
    XMVECTOR look = XMLoadFloat3(&mLook);
    XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
    look = XMVector3TransformNormal(look, rotation);
    XMStoreFloat3(&mLook, look);
}

void Camera::RotateY(float angle)
{
    XMVECTOR look = XMLoadFloat3(&mLook);
    XMMATRIX rotation = XMMatrixRotationY(angle);
    look = XMVector3TransformNormal(look, rotation);
    XMStoreFloat3(&mLook, look);
}

XMMATRIX Camera::GetViewMatrix()
{
    return mView;
}

XMMATRIX Camera::GetProjectionMatrix()
{
    return mProjectionMatrix;
}

void Camera::UpdateViewMatrix()
{

    XMVECTOR right = XMLoadFloat3(&mRight);
    XMVECTOR up = XMLoadFloat3(&mUp);
    XMVECTOR look = XMLoadFloat3(&mLook);
    XMVECTOR pos = XMLoadFloat3(&mPosition);

    look = XMVector3Normalize(look);
    up = XMVector3Normalize(XMVector3Cross(look, right));
    right = XMVector3Cross(up, look);

    float x = -XMVectorGetX(XMVector3Dot(pos, right));
    float y = -XMVectorGetX(XMVector3Dot(pos, up));
    float z = -XMVectorGetX(XMVector3Dot(pos, look));

    mView(0, 0) = XMVectorGetX(right);
    mView(1, 0) = XMVectorGetY(right);
    mView(2, 0) = XMVectorGetZ(right);
    mView(3, 0) = x;

    mView(0, 1) = XMVectorGetX(up);
    mView(1, 1) = XMVectorGetY(up);
    mView(2, 1) = XMVectorGetZ(up);
    mView(3, 1) = y;

    mView(0, 2) = XMVectorGetX(look);
    mView(1, 2) = XMVectorGetY(look);
    mView(2, 2) = XMVectorGetZ(look);
    mView(3, 2) = z;

    mView(0, 3) = 0.0f;
    mView(1, 3) = 0.0f;
    mView(2, 3) = 0.0f;
    mView(3, 3) = 1.0f;
}