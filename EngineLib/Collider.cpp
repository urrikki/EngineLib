#include "pch.h"
#include "Collider.h"

Collider::Collider() {}

Collider::~Collider() {}

void Collider::setCenterPos(XMFLOAT3 otherCenter) {       
    center.x = otherCenter.x;
    center.y = otherCenter.y;
    center.z = otherCenter.z;     
}

bool Collider::distanceBetweenCenter(XMFLOAT3 objCenter) {
    float dx = center.x - objCenter.x;
    float dy = center.y - objCenter.y;
    float dz = center.z - objCenter.z;
    float valDistance = sqrt(dx * dx + dy * dy + dz * dz);

    if (valDistance <= 0)
    {
        return true;
    }

    return false;
}


void Collider::watchCollideType(bool newType) {
    if (newType == false)
    {
        if (Collidetype == State::IsColliding) {
            Collidetype = State::haveCollid;
        }
        else {
            Collidetype = State::NoCollid;
        }
    }
    else if (newType == true) {
        Collidetype = State::IsColliding;
    }
}

State Collider::returnState()
{
    return Collidetype;
}