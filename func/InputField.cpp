#include "InputField.h"
#include "Variables.h"

//Constructors
InputField::InputField() {
    x = 5;
    y = screenHeight / 2 - screenHeight*0.63f * 0.15f;
    width = screenWidth*0.25f - 10;
    height = screenHeight*0.63f * 0.15f;
    bgColor = WHITE;
    textColor = BLACK;
    label = "v = ";
    textLength = 0;
    text[0] = '\0';
    focus = false;
}

InputField::InputField(int x, int y, int width, int height, const char* label):
        x(x), y(y), width(width), height(height), bgColor(WHITE), textColor(BLACK), focus(false), label(label), textLength(0){ text[0] = '\0';}

InputField::~InputField(){
    text[0] = '\0';
}

//Event
void InputField::Draw(){
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangleLines(x, y, width, height, textColor);
    DrawText(label, x + 10, y + height / 2 - 15, 30, textColor); //MeasureText(label, 20)
    if(focus){
            if((GetTime() / 0.6) - (int)(GetTime() / 0.6) < 0.6){
                DrawLine(x + width / 2 + MeasureText(text, 30) / 3 * 2+ 2, y + height /3, x + width / 2 + MeasureText(text, 30) / 3 * 2+ 2, y + height * 2 / 3, textColor);
            }
        
    }
    if(textLength){
        DrawText(text, x + width / 2 - MeasureText(text, 30) / 3 , y + height / 2 - 15 , 30, textColor);
    }
}

void InputField::Update(){
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        focus = (mousePos.x >= x && mousePos.x <=  + width && mousePos.y >= y && mousePos.y <= y + height);
    }

    if(focus){
        int key = GetCharPressed();
        while(key){
             if ((key >= 48) && (key <= 57) && (textLength < 5)) {
                text[textLength] = (char)key;
                textLength++;
                text[textLength] = '\0';
            }
            key = GetCharPressed();
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            if(textLength > 0){
                textLength--;
                text[textLength] = '\0';
            }
        }
    }
}
bool InputField::isFocus(){
    return focus;
}