#include "Camera.h"

Camera::Camera()
{
    mPosition = { 0.0f, 0.0f, 0.0f };
    mRotation = { 0.0f, 0.0f, 0.0f };
    UpdateViewMatrix();
}

Camera::~Camera() {}

void Camera::SetFOV(float fovDegrees, float screenWidth, float screenHeight)
{
    float aspectRatio = screenWidth / screenHeight;
    fovDegrees = fovDegrees * (PI / 180);

    DirectX::XMStoreFloat4x4(&mProjectionMatrix, DirectX::XMMatrixPerspectiveFovRH(fovDegrees, aspectRatio, 0.1f, 100.0f));
}

void Camera::Update(float deltaTime)
{
    //POINT mouseDelta = Input::getMouseDelta();

    //// Adjust camera rotation based on mouse movement
    //mRotation.x += static_cast<float>(mouseDelta.y) * 0.1f;
    //mRotation.y += static_cast<float>(mouseDelta.x) * 0.1f;

    //// Clamp vertical rotation to avoid flipping
    //mRotation.x = std::max(-89.0f, std::min(89.0f, mRotation.x));

    //UpdateViewMatrix();
}

void Camera::SetPosition(const DirectX::XMFLOAT3& position)
{
    mPosition = position;
    UpdateViewMatrix();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
    return DirectX::XMLoadFloat4x4(&mViewMatrix);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
    return DirectX::XMLoadFloat4x4(&mProjectionMatrix);
}

void Camera::UpdateViewMatrix()
{
    DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(mPosition.x, mPosition.y, mPosition.z, 1.0f);
    DirectX::XMVECTOR lookAtDirection = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Apply rotation to the lookAtDirection
    DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
        DirectX::XMConvertToRadians(mRotation.x),
        DirectX::XMConvertToRadians(mRotation.y),
        0.0f);

    lookAtDirection = DirectX::XMVector3TransformCoord(lookAtDirection, rotationMatrix);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtRH(eyePosition, DirectX::XMVectorAdd(eyePosition, lookAtDirection), upDirection);

    DirectX::XMStoreFloat4x4(&mViewMatrix, viewMatrix);
}
