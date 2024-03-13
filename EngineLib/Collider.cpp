#include "pch.h"
#include "Collider.h"

Collider::Collider() {}

Collider::~Collider() {}

void Collider::Update() {

}

XMFLOAT3 Collider::findCenterPos(const array<Vertex, 8>& vertices) {    
    XMFLOAT3 center = XMFLOAT3(0.0f, 0.0f, 0.0f);

    for (const auto& vertex : vertices)
    {
        center.x += vertex.Pos.x;
        center.y += vertex.Pos.y;
        center.z += vertex.Pos.z;
    }

    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();

    return center;  
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