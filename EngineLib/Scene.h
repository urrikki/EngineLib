#pragma once

#include "pch.h"

class Scene {
public:
    virtual ~Scene() {}

    virtual void Load() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};