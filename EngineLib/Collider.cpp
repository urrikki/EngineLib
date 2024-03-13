#include "pch.h"
#include "Collider.h"

Collider::Collider() {}

Collider::~Collider() {}

void Collider::Update() {

}

void Collider::setCenterPos(XMFLOAT3 otherCenter) {       
    center.x = otherCenter.x;
    center.y = otherCenter.y;
    center.z = otherCenter.z;     
}

bool Collider::distanceBetweenCenter(XMFLOAT3 objCenter) {
    double dx = center.x - objCenter.x;
    double dy = center.y - objCenter.y;
    double dz = center.z - objCenter.z;
    double valDistance = sqrt(dx * dx + dy * dy + dz * dz);

    if (valDistance <= 0)
    {
        return true;
    }

    return false;
}