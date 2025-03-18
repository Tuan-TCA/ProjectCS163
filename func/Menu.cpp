#include "Menu.h"
#include "Variables.h"
#include<bits/stdc++.h>

void Menu::init() {
    SwitchTheme = SwitchButton(screenWidth*0.9f, screenHeight*0.016f, screenWidth*0.082f, screenWidth*0.05f, "", MyColor1, MyColor1, WHITE);
    font = LoadFont("res/font/romulus.png");
    buttons = {
        AnimatedButton(screenWidth*0.4, screenHeight*0.3, screenWidth*0.2, screenHeight*0.1, "Linked List", MyColor1, MyColor1, WHITE),
        AnimatedButton(screenWidth*0.4, screenHeight*0.45, screenWidth*0.2, screenHeight*0.1, "Hash Table", MyColor1, MyColor1, WHITE),
        AnimatedButton(screenWidth*0.4, screenHeight*0.60, screenWidth*0.2, screenHeight*0.1, "Tree", MyColor1, MyColor1, WHITE),
        AnimatedButton(screenWidth*0.4, screenHeight*0.75, screenWidth*0.2, screenHeight*0.1, "Graph", MyColor1, MyColor1, WHITE)
    };
}

void Menu::draw() {
    ClearBackground(MyColor1);
    string title = "Data Visualization";
    DrawRectangle(0, 0, screenWidth, screenHeight, MyColor3);
    DrawTextEx(font, title.c_str(), {screenWidth*0.2f, screenHeight*0.1f}, screenWidth * 0.0667, 5, MyColor4);
    DrawTextEx(font, "\n\nCS163 - GROUP 6", {screenWidth*0.3f, screenHeight*0.1f}, screenWidth * 0.04, 5, MyColor4);
    SwitchTheme.Draw();
    for(auto &button : buttons) {
        button.Update();
        button.Draw(MyColor1);
    }
}

int Menu::modeRetrieve() {
    for(int i = 0; i < buttons.size(); i++) {
        if(buttons[i].IsClicked()) {
            return i+1;
        }
    }
    return -1;
}