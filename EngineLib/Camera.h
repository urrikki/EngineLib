#pragma once

#include "framework.h"
#include "Input.h"
#include "ShapeApp.h"

using namespace DirectX;

class Camera
{
public:
    Camera();
    ~Camera();

    void Update(float deltaTime);

    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);

    void Pitch(float angle);
    void RotateY(float angle);

    XMMATRIX GetViewMatrix();
    XMMATRIX GetProjectionMatrix();

private:
    XMFLOAT3 mPosition;
    XMFLOAT3 mRight;
    XMFLOAT3 mUp;
    XMFLOAT3 mLook;

    float mFOV;
    float mAspect;
    float mNearPlane;
    float mFarPlane;

    XMMATRIX mViewMatrix;
    XMMATRIX mProjectionMatrix;

    float mMouseSensitivity;

    void UpdateViewMatrix();
    DirectX::XMFLOAT4X4 mView = ;

};
