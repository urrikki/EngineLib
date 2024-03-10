#include "MenuScene.h"
#include <iostream>

MenuScene::MenuScene() {}

MenuScene::~MenuScene() {}

void MenuScene::Load() {
}

void MenuScene::Update(float deltaTime) {
    Input::update(/* handle de la fenêtre */)

    if (Input::isMouseButtonPressed(0)) {
        POINT mousePos = Input::getMouseDelta();
        // Vérifier si les coordonnées du clic sont à l'intérieur des coordonnées du bouton "Jouer"
        // Si oui, passer à la scène de jeu
        // Sinon, vérifier si les coordonnées sont à l'intérieur des coordonnées du bouton "Quitter"
        // Si oui, fermer la fenêtre
        sceneManager.SetActiveScene(std::make_shared<GameScene>());
    }
}

void MenuScene::Render() {
}
