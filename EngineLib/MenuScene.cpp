#include "MenuScene.h"
#include <iostream>

MenuScene::MenuScene() {}

MenuScene::~MenuScene() {}

void MenuScene::Load() {
}

void MenuScene::Update(float deltaTime) {
    Input::update(/* handle de la fen�tre */)

    if (Input::isMouseButtonPressed(0)) {
        POINT mousePos = Input::getMouseDelta();
        // V�rifier si les coordonn�es du clic sont � l'int�rieur des coordonn�es du bouton "Jouer"
        // Si oui, passer � la sc�ne de jeu
        // Sinon, v�rifier si les coordonn�es sont � l'int�rieur des coordonn�es du bouton "Quitter"
        // Si oui, fermer la fen�tre
        sceneManager.SetActiveScene(std::make_shared<GameScene>());
    }
}

void MenuScene::Render() {
}
