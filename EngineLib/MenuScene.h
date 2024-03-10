#pragma once
#include "Scene.h"
#include"Input.h"

class MenuScene : public Scene {
public:
    MenuScene();
    virtual ~MenuScene();

    virtual void Load() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:

};