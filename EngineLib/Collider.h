#pragma once
#include "pch.h"
#include "Component.h"

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class Collider : public Component
{
public :
	Collider();
	~Collider();
	XMFLOAT3 findCenterPos(const array<Vertex, 8>& vertices);
	XMFLOAT3 center;
	bool distanceBetweenCenter(XMFLOAT3 center);
	void Update() override;
};