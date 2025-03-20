#pragma once

#include<raylib.h>
#include "Button.h"
#include "Variables.h"
#include<bits/stdc++.h>
#include "InputField.h"
using namespace std;


enum class Operation {
    Insert,
    Create,
    Delete,
    Search
};

enum class InputType {
    Random,
    Keyboard,
    File
};
class Page {
public:
    Operation currentOperation;
    InputType currentInput;
    //input
    vector<string> options = {"RANDOM", "KEYBOARD", "FILE"};
    int selectedIndex;  // position hiện tại
    Button optionButton; // op hiện tại
    Button prevButton;  // chuyển trái
    Button nextButton; 

    Button insertButton, createButton, deleteButton, searchButton;
    vector<Button> operationButtons;
    vector<Vector2> subButtonPosition;

    bool isInsertExpanded = false; 
    bool isCreateExpanded = false; 
    bool isDeleteExpanded = false;  
    bool isSearchExpanded = false;  

    MyRec head;
    Rectangle bottom;
    Rectangle side;
    Button Ok;
    TextBox textbox;

    ButtonFromImage home, home2;
    Texture2D background1, background2;

    void init();
    void draw();
    void event();
    void reset();

    void handleInput();
    // FUNC func = FUNC::NONE;

};