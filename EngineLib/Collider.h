#pragma once
#include "pch.h"
#include "Component.h"

enum State
{
	NoCollid,
	IsColliding,
	haveCollid,
};

class Collider : public Component
{
public:

	Collider();
	~Collider();
	void setCenterPos(XMFLOAT3 center);
	XMFLOAT3 center;
	bool distanceBetweenCenter(XMFLOAT3 center);
	void watchCollideType(bool newType);
	State returnState();
	State Collidetype;
};



