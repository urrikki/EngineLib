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

    mPosition.x += mRight.x * mouseDelta.x * mMouseSensitivity;
    mPosition.y += mRight.y * mouseDelta.x * mMouseSensitivity;
    mPosition.z += mRight.z * mouseDelta.x * mMouseSensitivity;

    mPosition.x += mUp.x * mouseDelta.y * mMouseSensitivity;
    mPosition.y += mUp.y * mouseDelta.y * mMouseSensitivity;
    mPosition.z += mUp.z * mouseDelta.y * mMouseSensitivity;

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

XMMATRIX Camera::GetViewMatrix()
{
    return mViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix()
{
    return mProjectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    XMVECTOR pos = XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 0.0f);
    XMVECTOR right = XMVectorSet(mRight.x, mRight.y, mRight.z, 0.0f);
    XMVECTOR up = XMVectorSet(mUp.x, mUp.y, mUp.z, 0.0f);
    XMVECTOR look = XMVectorSet(mLook.x, mLook.y, mLook.z, 0.0f);

    look = XMVector3Normalize(look);
    up = XMVector3Normalize(XMVector3Cross(look, right));
    right = XMVector3Cross(up, look);

    float x = -XMVectorGetX(XMVector3Dot(pos, right));
    float y = -XMVectorGetX(XMVector3Dot(pos, up));
    float z = -XMVectorGetX(XMVector3Dot(pos, look));

    mRight = XMFLOAT3(XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right));
    mUp = XMFLOAT3(XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up));
    mLook = XMFLOAT3(XMVectorGetX(look), XMVectorGetY(look), XMVectorGetZ(look));

    mViewMatrix(0, 0) = mRight.x;
    mViewMatrix(1, 0) = mRight.y;
    mViewMatrix(2, 0) = mRight.z;
    mViewMatrix(3, 0) = x;

    mViewMatrix(0, 1) = mUp.x;
    mViewMatrix(1, 1) = mUp.y;
    mViewMatrix(2, 1) = mUp.z;
    mViewMatrix(3, 1) = y;

    mViewMatrix(0, 2) = mLook.x;
    mViewMatrix(1, 2) = mLook.y;
    mViewMatrix(2, 2) = mLook.z;
    mViewMatrix(3, 2) = z;

    mViewMatrix(0, 3) = 0.0f;
    mViewMatrix(1, 3) = 0.0f;
    mViewMatrix(2, 3) = 0.0f;
    mViewMatrix(3, 3) = 1.0f;
}
