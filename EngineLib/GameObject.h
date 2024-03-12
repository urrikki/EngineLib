#pragma once
#include "pch.h"
#include "Framework.h"
#include "Component.h"
#include "Transform.h"
#pragma once
#include "pch.h"
#include <vector>

class GameObject {
public:
	TRANSFORM Transform;

	GameObject();
	~GameObject();
	void Update();
	void setPos(float x , float y , float z);

	std::vector<Component*> tComponentList;

	template<typename T>
	void AddComponent() {

		T* pT = GetComponent<T>();
		if (!pT)
			pT = new T;
			tComponentList.push_back(pT);
	}

	template<typename T>
	void RemoveComponent()
	{
		T* pT = GetComponent<T>();
		if (pT)
		{
			delete pT;
			tComponentList.erase(pT);
		}
	}

	//GetComponent<ShaderReference>()
	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < tComponentList.size(); i++) {
			T* valReturn = dynamic_cast<T*>(tComponentList[i]);
			if (valReturn )
				return valReturn;
		}
		return nullptr;
	}
};