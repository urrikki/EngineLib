#include "pch.h"
//#include "Button.h"
//#include <iostream>
//
//Button::Button(RECT bounds, std::string text) : mBounds(bounds), mText(text) {
//}
//
//Button::~Button() {
//}
//
//bool Button::IsClicked() const {
//    POINT mousePos = Input::getMouseDelta();
//    if (Input::isMouseButtonPressed(0) && PtInRect(&mBounds, mousePos))
//        return true;
//    else
//        return false;
//}
//
//
//void Button::Render() const {
//    // Affichage du bouton (dessin, texte, etc.)
//    std::cout << "Rendering button with text: " << mText << std::endl;
//}
//
//std::string Button::GetText() const {
//    return mText;
//}
