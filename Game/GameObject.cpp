#include "pch.h"
#include "GameObject.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::Update() {
	for (int i = 0; i < tComponentList.size(); i++) {
		tComponentList[i]->Update();
	}
}

void GameObject::ResetPos() {}

