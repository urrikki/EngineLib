#include "pch.h"
#include "Transform.h"
#include <iostream>


void TRANSFORM::Identity()
{
    vSca.x = 1.0f;
    vSca.y = 1.0f;
    vSca.z = 1.0f;

    mSca._11 = 1.0f;
    mSca._12 = 0.0f;
    mSca._13 = 0.0f;
    mSca._14 = 0.0f;

    mSca._21 = 0.0f;
    mSca._22 = 1.0f;
    mSca._23 = 0.0f;
    mSca._24 = 0.0f;

    mSca._31 = 0.0f;
    mSca._32 = 0.0f;
    mSca._33 = 1.0f;
    mSca._34 = 0.0f;

    mSca._41 = 0.0f;
    mSca._42 = 0.0f;
    mSca._43 = 0.0f;
    mSca._44 = 1.0f;

    //XMStoreFloat4x4(&mSca, XMMatrixIdentity());

    vDir.x = 0.0f;
    vDir.y = 0.0f;
    vDir.z = 1.0f;

    vRight.x = 1.0f;
    vRight.y = 0.0f;
    vRight.z = 0.0f;

    vUp.x = 0.0f;
    vUp.y = 1.0f;
    vUp.z = 0.0f;

    qRot.x = 0.0f;
    qRot.y = 0.0f;
    qRot.z = 0.0f;
    qRot.w = 1.0f;

    mRot = mSca;

    vPos.x = 0.0f;
    vPos.y = 0.0f;
    vPos.z = 0.0f;

    mPos = mSca;

    matrix = mSca;

}

void TRANSFORM::Rotate(float fRoll, float fPitch, float fYaw)
{
    // Créer un quaternion pour chaque rotation (delta)
    XMVECTOR quatRot;

    XMFLOAT3 axisDir = vDir;
    XMVECTOR quatDir = XMQuaternionRotationAxis(XMLoadFloat3(&axisDir), fRoll);
    quatRot = quatDir;

    XMFLOAT3 axisRight = vRight;
    XMVECTOR quatRight = XMQuaternionRotationAxis(XMLoadFloat3(&axisRight), fPitch);
    quatRot = XMQuaternionMultiply(quatRot, quatRight);

    XMFLOAT3 axisUp = vUp;
    XMVECTOR quatUp = XMQuaternionRotationAxis(XMLoadFloat3(&axisUp), fYaw);
    quatRot = XMQuaternionMultiply(quatRot, quatUp);

    // Ajouter la rotation delta à la rotation actuelle de l'objet
    XMStoreFloat4(&qRot, XMQuaternionMultiply(XMLoadFloat4(&qRot), quatRot));

    // Convertir le quaternion en une matrice de rotation (magique)
    XMMATRIX matRot = XMMatrixRotationQuaternion(XMLoadFloat4(&qRot));

    //Passer la matrix en XMFLOAT4X4
    XMStoreFloat4x4(&mRot, matRot);
}

void TRANSFORM::Scaling(float fX, float fY, float fZ)
{
    vSca.x = fX;
    vSca.y = fY;
    vSca.z = fZ;
    XMMATRIX scaMat = XMMatrixScaling(fX, fY, fZ);
    XMStoreFloat4x4(&mSca, scaMat);
}

void TRANSFORM::Translation(float fX, float fY, float fZ)
{
    vPos.x = fX;
    vPos.y = fY;
    vPos.z = fZ;
    XMMATRIX traMat = XMMatrixTranslation(fX, fY, fZ);
    XMStoreFloat4x4(&mPos, traMat);
}

float TRANSFORM::SpeedUntilMax(float* fValue, float fSpeed, float fMax)
{
    if (*fValue < fMax)
    {
        *fValue += fSpeed;
    }
    return *fValue;
}

void TRANSFORM::UpdateWorld()
{
    XMMATRIX worldMat = XMLoadFloat4x4(&mSca) * XMLoadFloat4x4(&mRot) * XMLoadFloat4x4(&mPos);
    XMStoreFloat4x4(&matrix, worldMat);
}
