#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() {}

GameObject::~GameObject()
{
	assert(tComponentList.size() == 0);
}

void GameObject::Update() {
	for (int i = 0; i < tComponentList.size(); i++) {
		tComponentList[i]->Update();
	}
}

void GameObject::setPos(float x , float y, float z) {
	Transform.Translation(x, y, z);
	Transform.UpdateWorld();
}

