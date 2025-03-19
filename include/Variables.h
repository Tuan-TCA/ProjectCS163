#pragma once

#include<raylib.h>
#include<bits/stdc++.h>

enum class MODE {
    MENU, LL, HASHTB, AVL, GRAPH    
};

enum class FUNC {
    CREATE, INSERT, SEARCH, DELETE, UPDATE
};

extern FUNC func;
extern MODE mode;
extern bool switchState; 
extern const int screenWidth, screenHeight;

extern Color MyColor1;
extern Color MyColor2;
extern Color MyColor3;
extern Color MyColor4;
extern Color MyColor5;
extern Texture2D resizedImage(const char* imagePath, float width, float height);
extern void UpdateColorsBasedOnSwitchState();