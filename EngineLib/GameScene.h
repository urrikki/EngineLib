#pragma once
#include "Scene.h"

class GameScene : public Scene {
public:
    GameScene();
    virtual ~GameScene();

    virtual void Load() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;

private:

};