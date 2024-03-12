#pragma once

#include "framework.h"
#include "Component.h"
#include "Input.h"

class Camera : public Component
{
public:
    Camera();
    ~Camera();

    void SetFOV(float fovDegrees, float screenWidth, float screenHeight);
    void Update(float deltaTime);

    void SetPosition(const DirectX::XMFLOAT3& position);
    void SetRotation(const DirectX::XMFLOAT3& rotation);

    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetProjectionMatrix() const;

private:
    const float PI = 3.1415926535f;

    DirectX::XMFLOAT3 mPosition;
    DirectX::XMFLOAT3 mRotation;
    DirectX::XMFLOAT4X4 mViewMatrix;
    DirectX::XMFLOAT4X4 mProjectionMatrix;

    void UpdateViewMatrix();
};

