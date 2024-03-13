#pragma once
#include "pch.h"
#include "Component.h"

class Collider : public Component
{
public:

	Collider();
	~Collider();
	

	void setCenterPos(XMFLOAT3 center);
	XMFLOAT3 center;
	bool distanceBetweenCenter(XMFLOAT3 center);
	void Update() override;
};



