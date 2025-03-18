#pragma once
#include "raylib.h"
#include "Variables.h"
#include<bits/stdc++.h>
#include <string>
using namespace std;

class MyRec {
private:
    Rectangle bounds;
    Color color;
    Color textColor;
    string label;

public:
    MyRec() {};
    MyRec(float x, float y, float width, float height, string labelText, Color color, Color textCol) {
        bounds = {x, y, width, height};
        label = labelText;
        color = color;
        textColor = textCol;
    }

    void Draw() {
        DrawRectangleRec(bounds, color);
        int textWidth = MeasureText(label.c_str(), 40);
        DrawText(label.c_str(), bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 40) / 2, 40, textColor);
    }

    void DrawRounded(){
        DrawRectangleRounded(bounds, 20, 0, MyColor2);
        DrawText(label.c_str(), bounds.x + (bounds.width - MeasureText(label.c_str(), 40)) / 2, bounds.y + (bounds.height - 40) / 2, 40, textColor);
    }

    void Draw(Color color, string Mode){
        DrawRectangleRec(bounds, color);
        int textWidth = MeasureText(Mode.c_str(), 40);
        DrawText(Mode.c_str(), bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 40) / 2, 40, textColor);
    }
};

class Button {
public:
    Rectangle bounds;
    Color color;
    Color hoverColor;
    Color textColor;
    const char* label;
    bool isHovered;
    Texture2D image;

    Button() {
        bounds = {0,0,0,0};
        label = "";
        color = WHITE;
        hoverColor = WHITE;
        textColor = WHITE;
        isHovered = false;
    }

    Button(float x, float y, float width, float height, const char* labelText, Color buttonColor, Color hoverCol, Color textCol) {
        bounds = {x, y, width, height};
        label = labelText;
        color = buttonColor;
        hoverColor = hoverCol;
        textColor = textCol;
        isHovered = false;
    }

    virtual void Update() {
        Vector2 mousePoint = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePoint, bounds);
    }

    virtual void Draw() {
        Update();
        DrawRectangleRec(bounds, isHovered ? hoverColor : color);
        int textWidth = MeasureText(label, 20);
        DrawText(label, bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, textColor);
    }

    virtual void Draw(Color HoverColor, Color color){
        Update();
        DrawRectangleRec(bounds, isHovered ? HoverColor : color);
        int textWidth = MeasureText(label, 20);
        DrawText(label, bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, textColor);
    }
    void DrawRounded(){
        Update();
        DrawRectangleRounded(bounds, 20, 0,isHovered ? MyColor2 : MyColor1);
        int textWidth = MeasureText(label, 20);
        DrawText(label, bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, textColor);
    }

    void DrawText2D () {
        Update();
        Rectangle sourceRec = {0,0, (float) image.width, (float) image.height};
        Vector2 origin = {0,0};
        DrawTexturePro(image, sourceRec, bounds, origin, 0.0f, isHovered ? LIGHTGRAY : YELLOW);
    }

    Rectangle getBound(){
        return bounds;
    }
    bool isOver(){
        return isHovered;
    }
    bool IsClicked() {
        Update();
        return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
};

class TextBox: public Button {
public:
    string inputText;
    bool active;
    vector<int> nums;

    TextBox() : Button() {
        inputText = "";
        active = false;
        nums.clear();
    }

    TextBox(float x, float y, float width, float height, const char* labelText, 
            Color buttonColor, Color hoverCol, Color textCol)
        : Button(x, y, width, height, labelText, buttonColor, hoverCol, textCol) { 
        inputText = "";  
        active = false;  
        nums.clear();    
    }

    void Draw() override {
        Button::Draw();
        string displayText = inputText;
        if (active && ((int)(GetTime() * 2) % 2 == 0)) { // Nhấp nháy con trỏ
            displayText += "|";
        }
        DrawText(displayText.c_str(), bounds.x + bounds.width / 2 - MeasureText(displayText.c_str(), 40) / 2, bounds.y + bounds.height / 3 - 3 , 40, textColor);
    }

    void HandleInput(bool Add, bool Del) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        active = (mousePos.x >= bounds.x && mousePos.x <=  bounds.x + bounds.width && mousePos.y >= bounds.y && mousePos.y <= bounds.y + bounds.height);
        }
        if (active || Add || Del) {
            int key = GetCharPressed();
            while (key > 0 && inputText.size() < 7) {
                if (key >= '0' && key <= '9') {
                    inputText += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !inputText.empty()) {
                inputText.pop_back();
            }

            if ((Add || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && !inputText.empty()) {
                nums.push_back(stoi(inputText));
                TraceLog(LOG_INFO, inputText.c_str());
                inputText = "";
            }
         
            if ((Del) && !inputText.empty()) {
                auto it = find(nums.begin(), nums.end(), stoi(inputText));
                if(it!=nums.end())
                    nums.erase(it);
                inputText = "";
            }
        }
    }

    void resetTextbox(){
        inputText = "";
        active = false;
        nums.clear();
    }
    
};

class ButtonFromImage : public Button {
private:
    Texture2D texture;
    Texture2D textureOver;
public:
    ButtonFromImage() : Button(0, 0, 0, 0, "", RAYWHITE, RAYWHITE, RAYWHITE), texture({ 0 }), textureOver({ 0 }) {}
    ButtonFromImage(const char* imagePath, const char* imageOverPath, float x, float y, float width, float height)
        : Button(x, y, width, height, "", RAYWHITE, RAYWHITE, RAYWHITE) {
        texture = resizedImage(imagePath, width, height);
        textureOver = resizedImage(imageOverPath, width, height);
    }

    void Draw() override {
        Update();
        DrawTexture(!isOver() ? texture : textureOver, bounds.x, bounds.y, WHITE);
    }
};

class AnimatedButton : public Button {
public:
    bool isScaling;
    float scaleFactor;
    float scaleSpeed;
    float colorAlpha;
    float colorAlphaSpeed;

    AnimatedButton(){}
    AnimatedButton(float x, float y, float width, float height, const char* labelText, Color buttonColor, Color hoverCol, Color textCol)
        : Button(x, y, width, height, labelText, buttonColor, hoverCol, textCol) {
        isScaling = false;
        scaleFactor = 1.0f;
        scaleSpeed = 1.0f;
        colorAlpha = 1.0f;
        colorAlphaSpeed = 1.0f;
    }

    void Update() override {
        Button::Update(); 

        float deltaTime = GetFrameTime();
        if (isHovered && !isScaling) {
            isScaling = true;
        } else if (!isHovered && isScaling) {
            isScaling = true;
        }

        if (isScaling) {
            if (isHovered) {
                scaleFactor += scaleSpeed * deltaTime;
                colorAlpha -= colorAlphaSpeed * deltaTime;
                if (scaleFactor > 1.2f) scaleFactor = 1.2f;
                if (colorAlpha < 0.5f) colorAlpha = 0.5f;
            } else {
                scaleFactor -= scaleSpeed * deltaTime;
                colorAlpha += colorAlphaSpeed * deltaTime;
                if (scaleFactor < 1.0f) {
                    scaleFactor = 1.0f;
                    isScaling = false;
                }
                if (colorAlpha > 1.0f) {
                    colorAlpha = 1.0f;
                    isScaling = false;
                }
            }
        }
    }

    void Draw() override {
        Rectangle scaledBounds = {
            bounds.x - (bounds.width * (scaleFactor - 1.0f) / 2.0f),
            bounds.y - (bounds.height * (scaleFactor - 1.0f) / 2.0f),
            bounds.width * scaleFactor,
            bounds.height * scaleFactor
        };

        Color drawColor = color;
        drawColor.a = static_cast<unsigned char>(colorAlpha * 255);

        DrawRectangleRounded(scaledBounds, 17, 2, drawColor);
        int textWidth = MeasureText(label, 20);
        DrawText(label, bounds.x - (bounds.width * (scaleFactor - 1.0f) / 2.0f) + (bounds.width * scaleFactor - textWidth ) / 2, bounds.y  - (bounds.height * (scaleFactor - 1.0f) / 2.0f) + (bounds.height * scaleFactor - 20) / 2, 20 * scaleFactor, textColor);
    }

    void Draw(Color color) {
        Rectangle scaledBounds = {
            bounds.x - (bounds.width * (scaleFactor - 1.0f) / 2.0f),
            bounds.y - (bounds.height * (scaleFactor - 1.0f) / 2.0f),
            bounds.width * scaleFactor,
            bounds.height * scaleFactor
        };

        Color drawColor = color;
        drawColor.a = static_cast<unsigned char>(colorAlpha * 255);

        DrawRectangleRounded(scaledBounds, 17, 2, drawColor);
        int textWidth = MeasureText(label, 20);
        DrawText(label, bounds.x - (bounds.width * (scaleFactor - 1.0f) / 2.0f) + (bounds.width * scaleFactor - textWidth ) / 2, bounds.y  - (bounds.height * (scaleFactor - 1.0f) / 2.0f) + (bounds.height * scaleFactor - 20) / 2, 20 * scaleFactor, textColor);
    }
};

class SwitchButton : public Button {
public:
    float switchPos;  
    bool isSwitching; 
    float switchSpeed; 

    SwitchButton(){}
    SwitchButton(float x, float y, float width, float height, const char* labelText, Color buttonColor, Color hoverCol, Color textCol)
        : Button(x, y, width, height, labelText, buttonColor, hoverCol, textCol) {
        switchPos = 0.0f; 
        switchState = false; // is turning off
        isSwitching = false; 
        switchSpeed = 0.05f;
    }


    void UpdateSwitch() {
        Vector2 mousePos = GetMousePosition();

       
        if (IsClicked()) {
            switchState = !switchState; 
            UpdateColorsBasedOnSwitchState();
            isSwitching = true;  
        }

        if (isSwitching) {
            if (switchState && switchPos < 1.0f) {
                switchPos += switchSpeed;
                if (switchPos >= 1.0f) {
                    switchPos = 1.0f;
                    isSwitching = false; 
                }
            }
            else if (!switchState && switchPos > 0.0f) {
                switchPos -= switchSpeed;
                if (switchPos <= 0.0f) {
                    switchPos = 0.0f;
                    isSwitching = false;  
                }
            }
        }
    }

    void Draw() override {
        UpdateSwitch();
        
        DrawRectangleRounded(bounds, bounds.height / 2, 10, MyColor4);
        DrawCircle(bounds.x + bounds.width * 0.3f +switchPos * (bounds.width - bounds.height), bounds.y + bounds.height / 2, bounds.height / 2 - 5, WHITE);
    }
};