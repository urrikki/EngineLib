#pragma once

#include "pch.h"

class Scene {
public:
    virtual ~Scene() {}

    virtual void Initialize() = 0;
    virtual void Load() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};

//class Button {
//private:
//    RECT bounds;
//    std::string text;
//
//public:
//    Button(RECT bounds, std::string text) : bounds(bounds), text(text) {}
//
//    bool IsClicked(POINT mousePos) {
//        return PtInRect(&bounds, mousePos) != 0;
//    }
//
//    void Render() {
//        // Dessiner le bouton à l'écran en utilisant les informations de la zone et du texte
//        // par exemple, utiliser les fonctions de rendu de votre moteur graphique
//    }
//
//};