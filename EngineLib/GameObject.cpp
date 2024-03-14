#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() {
	Transform.Identity();	
}

GameObject::~GameObject()
{
	assert(tComponentList.size() == 0);
}

void GameObject::Update() {
	for (int i = 0; i < tComponentList.size(); i++) {
		tComponentList[i]->Update();
	}
}

