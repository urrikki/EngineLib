#pragma once
#pragma once

#include "Framework.h"
#include "Component.h"
#include "Transform.h"
#include <iostream>
#include <vector>

class GameObject {
public:
	TRANSFORM Transform;

	GameObject();
	~GameObject();
	void Update();
	void ResetPos();

	std::vector<Component*> tComponentList;

	template<typename T>
	void AddComponent()
	{
		for (int i = 0; i < tComponentList.size(); i++) {
			if (dynamic_cast<T>(tComponentList[i]) != nullptr) {
				tComponentList.push_back(tComponentList[i]);
			}
		}
	}

	template<typename T>
	void RemoveComponent()
	{
		for (int i = 0; i < tComponentList.size(); i++) {
			if (dynamic_cast<T>(tComponentList[i]) != nullptr) {
				tComponentList.erase(tComponentList[i]);
			}
		}
	}

	//GetComponent<ShaderReference>()
	template<typename T>
	T GetComponent()
	{
		for (int i = 0; i < tComponentList.size(); i++) {
			if (dynamic_cast<T>(tComponentList[i]) != nullptr) {
				return tComponentList[i];
			}
		}
		return nullptr;
	}
};