#pragma once
#include <raylib.h>

class InputField{
    private:
        int x, y;
        int width, height;
        Color bgColor, textColor;
        const char* label;
        bool focus;

    public:
        InputField();
        ~InputField();
        int textLength;
        char text[5]; //max input value = 5

        InputField(int x, int y, int width, int height, const char* label);
        void Draw();
         void Update();
        bool isFocus();
        const char* getText();
};

