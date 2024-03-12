#pragma once
#include "Framework.h"

struct TRANSFORM
{
    /*VECTOR3*/ XMFLOAT3 vSca;
    /*MATRIX*/ XMFLOAT4X4 mSca;

    /*VECTOR3*/ XMFLOAT3 vDir;
    /*VECTOR3*/ XMFLOAT3 vRight;
    /*VECTOR3*/ XMFLOAT3 vUp;
    /*QUATERNION*/ XMFLOAT4 qRot;
    // permet de faire tourner un objet sans casser les axes ( x y z w) faire tourner les axes (principal pour les rotations)
    /*MATRIX*/ XMFLOAT4X4 mRot;

    /*VECTOR3*/ XMFLOAT3 vPos;
    /*MATRIX*/ XMFLOAT4X4 mPos;

    /*MATRIX*/ XMFLOAT4X4 matrix;

    //XMMATRIX view;
    //XMMATRIX proj;

    void Identity();
    void Rotate(float fRoll, float fYaw, float fPitch);
    void Scaling(float fX, float fY, float fZ);
    void Translation(float fX, float fY, float fZ);
    float SpeedUntilMax(float* fValue, float fSpeed, float fMax);
    void UpdateWorld();
};