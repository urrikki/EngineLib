#include "pch.h"
//#include "MenuScene.h"
//#include <iostream>
//
//MenuScene::MenuScene() {}
//
//MenuScene::~MenuScene() {}
//
//void MenuScene::Initialize() {
//    RECT playButtonBounds = { /* position x, position y, largeur, hauteur */ };
//    buttons.push_back(Button(playButtonBounds, "Jouer"));
//
//    RECT quitButtonBounds = { /* position x, position y, largeur, hauteur */ };
//    buttons.push_back(Button(quitButtonBounds, "Quitter"));
//}
//
//void MenuScene::Update(float deltaTime, HWND hWnd) {
//    Input::update(hWnd);
//
//    if (Input::isMouseButtonPressed(0)) {
//        for (const auto& button : buttons) {
//            if (button.IsClicked()) {
//                if (button.GetText() == "Jouer") {
//                    //sceneManager->SetActiveScene(std::make_shared<GameScene>());
//                }
//                else if (button.GetText() == "Quitter") {
//                    PostQuitMessage(0);
//                }
//            }
//        }
//    }
//
//}
//
//void MenuScene::Render() {
//    for (const auto& button : buttons) {
//        button.Render();
//    }
//}