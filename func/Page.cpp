#include "Page.h"
#include "Variables.h"
#include <raylib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>
#include "ControlAnimation.h"
using namespace std;

int lastInsertedKey;

string getMODE() {
    if(mode == MODE::LL) return "Linked List";
    if(mode == MODE::HASHTB) return "Hash Table";
    if(mode == MODE::AVL) return "AVL Tree";
    if(mode == MODE::GRAPH) return "Graph";
    return "";
}


void Page::init() {

    selectedInputIndex = 0;
    InputOptionButton = Button(screenWidth * 0.24f * 0.15f,screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 , screenWidth*0.24f * 0.7f, screenHeight*0.63f * 0.15f, InputOptions[selectedInputIndex].c_str(), WHITE, LIGHTGRAY, MyColor5);
    InputPrevButton = Button(5,screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 ,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f, "<", WHITE, LIGHTGRAY, MyColor5);
    InputNextButton = Button(screenWidth*0.24f * 0.85f + 5, screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 ,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f, ">", WHITE, LIGHTGRAY, MyColor5);
    currentInput = InputType::Keyboard;

    selectedOperationIndex = 0;
    OperationOptionButton = Button(screenWidth * 0.24f * 0.15f, screenHeight / 2 - screenHeight*0.63f * 0.5f + 5, screenWidth*0.24f * 0.7f, screenHeight*0.63f * 0.15f, OperationOptions[selectedOperationIndex].c_str(), MyColor1, Fade(MyColor1, 0.8f), WHITE);
    OperationPrevButton = Button(5, screenHeight / 2 - screenHeight*0.63f * 0.5f + 5,screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f, "<", MyColor1, Fade(MyColor1, 0.8f), WHITE);
    OperationNextButton = Button(screenWidth*0.24f * 0.85f + 5,screenHeight / 2 - screenHeight*0.63f * 0.5f + 5,  screenWidth*0.24f * 0.15f - 10,screenHeight*0.63f * 0.15f, ">", MyColor1, Fade(MyColor1, 0.8f), WHITE);
    currentOperation = Operation::Insert;

    Ok = Button(10 + screenWidth*0.25f - 100, screenHeight / 2 - screenHeight*0.63f * 0.17f, 73, screenHeight*0.63f * 0.15f, "OK", MyColor1, MyColor2, WHITE);
    head = MyRec(0, 10, (float) screenWidth, screenHeight*0.08f, getMODE().c_str(), MyColor2, WHITE);
    home = ButtonFromImage("res/button/back.png", "res/button/back-isOver.png", screenWidth*0.016f, screenHeight*0.016f, screenWidth*0.05f, screenWidth*0.05f); 
    home2 = ButtonFromImage("res/button/homeII_1.png", "res/button/homeII_2.png", screenWidth*0.016f, screenHeight*0.016f, screenWidth*0.05f, screenWidth*0.05f); 

    isPlaying = false;
    back1 = ButtonFromImage("res/button/1-prev.png", "res/button/1-prev.png", screenWidth / 2 -   screenWidth * 0.05f * 3 / 2 - 20 , screenHeight*0.926f,  screenWidth * 0.04f, screenWidth*0.04f);
    next1 = ButtonFromImage("res/button/1-next.png", "res/button/1-next.png", screenWidth / 2 +   screenWidth * 0.05f , screenHeight*0.926f,  screenWidth * 0.04f, screenWidth*0.04f);
    play1 = ButtonFromImage("res/button/1-play.png", "res/button/1-play.png", screenWidth / 2 -   screenWidth * 0.05f / 2  , screenHeight*0.92f,  screenWidth * 0.05f, screenWidth*0.05f);
    pause1 = ButtonFromImage("res/button/1-pause.png", "res/button/1-pause.png", screenWidth / 2 -   screenWidth * 0.05f / 2  , screenHeight*0.92f,  screenWidth * 0.05f, screenWidth*0.05f);
    back2 = ButtonFromImage("res/button/2-prev.png", "res/button/2-prev.png", screenWidth / 2 -   screenWidth * 0.05f * 3 / 2 - 20 , screenHeight*0.926f,  screenWidth * 0.04f, screenWidth*0.04f);
    next2 = ButtonFromImage("res/button/2-next.png", "res/button/2-next.png", screenWidth / 2 +   screenWidth * 0.05f , screenHeight*0.926f,  screenWidth * 0.04f, screenWidth*0.04f);
    pause2 = ButtonFromImage("res/button/2-pause.png", "res/button/2-pause.png", screenWidth / 2 -   screenWidth * 0.05f / 2  , screenHeight*0.92f,  screenWidth * 0.05f, screenWidth*0.05f);
    play2 = ButtonFromImage("res/button/2-play.png", "res/button/2-play.png", screenWidth / 2 -   screenWidth * 0.05f / 2  , screenHeight*0.92f,  screenWidth * 0.05f, screenWidth*0.05f);
    timeSlider = Slider({screenWidth * 0.05f , screenHeight*0.936f,  screenWidth * 0.3f ,screenHeight * 0.095f / 3 * 0.9f}, 0.0f, 1.0f);
    
    speedSliding = MyRec(screenWidth * 0.723f , screenHeight*0.936f,  screenWidth * 0.182f * 0.38f,screenHeight * 0.095f / 3 * 0.9f, "", MyColor3, WHITE);
    background1 = resizedImage("res/BackGround.png", screenWidth, screenHeight);   
    background2 = resizedImage("res/background_theme2.png", screenWidth, screenHeight);    
    bottom = {0,screenHeight*0.905f,(float)screenWidth,screenHeight*0.095f};
    side = {0,screenHeight / 2 - screenHeight * 0.64f / 2,screenWidth*0.24f,screenHeight*0.64f};
    textbox = TextBox(5, screenHeight / 2 - screenHeight*0.63f * 0.17f, screenWidth*0.25f - 100, screenHeight*0.63f * 0.15f, "", WHITE, WHITE, BLACK);
    oldTextBox = TextBox(5, screenHeight / 2 - screenHeight*0.63f * 0.17f, screenWidth*0.08, screenHeight*0.63f * 0.15f, "old:      ", WHITE, WHITE, BLACK);
    newTextBox = TextBox(screenWidth*0.08f + 10, screenHeight / 2 - screenHeight*0.63f * 0.17f, screenWidth*0.08, screenHeight*0.63f * 0.15f, "new:       ", WHITE, WHITE, BLACK);
}

void Page::reset(){
    currentOperation = Operation::Insert;
    currentInput = InputType::Random;
    selectedInputIndex = 0;
    selectedOperationIndex = 0;
    textbox.resetTextbox();
    oldTextBox.resetTextbox();
    newTextBox.resetTextbox();
}

void Page::draw() {
    ClearBackground(RAYWHITE);
    DrawTexture(switchState ? background2 : background1, 0, 0, WHITE);
    head.Draw(MyColor2, getMODE());
    DrawRectangleRec(bottom, MyColor2);
    DrawRectangleRec(side, MyColor3);
    DrawRectangleRounded({screenWidth * 0.7f , screenHeight*0.934f , screenWidth * 0.182f,screenHeight * 0.095f / 3}, 20, 20, WHITE); //speed control
    speedSliding.DrawRounded(MyColor3);
    timeSlider.Draw();
    stringstream ss;
    ss << fixed << std::setprecision(1) << animationSpeed;
    std::string formattedSpeed = ss.str();
    formattedSpeed.push_back('x');
    DrawText(formattedSpeed.c_str(), screenWidth * 0.906f, screenHeight * 0.935f, 30, WHITE);

    if(currentInput != InputType::File){
        if (currentOperation == Operation::Update) {
        oldTextBox.Draw(20);  
        newTextBox.Draw(20); 
        }
        else {
            textbox.Draw();
        }
        Ok.Draw(MyColor1, MyColor2);
    }
    else{
        DrawRectangle(5, screenHeight / 2 - screenHeight*0.63f * 0.17f, screenWidth*0.24f - 10, screenHeight*0.63f * 0.15f, WHITE);
        DrawText("DROP FILE HERE", 30, screenHeight / 2 - screenHeight*0.63f * 0.118f, 25, GRAY);
        //drop file field
    }

    if(switchState){
        isPlaying ? play2.Draw() : pause2.Draw();
        back2.Draw();
        next2.Draw();
        home2.Draw(); }
    else{ 
        isPlaying ? play1.Draw() : pause1.Draw();
        back1.Draw();
        next1.Draw();
        home.Draw();
    }
    //Input
    InputOptionButton.Draw(LIGHTGRAY, WHITE);
    InputPrevButton.Draw(LIGHTGRAY, WHITE);
    InputNextButton.Draw(LIGHTGRAY, WHITE);

    //Operation
    OperationOptionButton.Draw( Fade(MyColor1, 0.8f), MyColor1);
    OperationPrevButton.Draw(Fade(MyColor1, 0.8f), MyColor1);
    OperationNextButton.Draw(Fade(MyColor1, 0.8f), MyColor1);
}

void Page::event() {
    if(!switchState ? home.IsClicked(): home2.IsClicked() ) {
        mode = MODE::MENU;

        reset();
        return;
    }

    if(mode != MODE::GRAPH) {handleInput();}

    //speed sliding event
    Vector2 mousePoint = GetMousePosition();
    float minX = screenWidth * 0.7f + 1;
    float maxX = screenWidth * 0.7f + screenWidth * 0.181f - speedSliding.bounds.width;
    if (CheckCollisionPointRec(mousePoint, speedSliding.bounds) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        float clampedMouseX = clamp(mousePoint.x - speedSliding.bounds.width / 2, minX, maxX);
        speedSliding.bounds.x = clampedMouseX;
    }
    animationSpeed = clamp(static_cast<float>((speedSliding.bounds.x - minX) * 5 / (maxX - minX)), 0.1f, 5.0f);
    animationSpeed = round(animationSpeed * 10.0) / 10.0; // round up to 1 decimal

    //time slider
    timeSlider.Update();

    if(play1.IsClicked() || play2.IsClicked() || pause1.IsClicked() || pause2.IsClicked() || IsKeyPressed(KEY_SPACE)){
        isPlaying = !isPlaying;
        if(!isPlaying) TraceLog(LOG_INFO, "is pausing");
        else TraceLog(LOG_INFO, "is playing");
    }

    //Operation event
   if (OperationPrevButton.IsClicked()) {

        selectedOperationIndex = (selectedOperationIndex - 1 + OperationOptions.size()) % OperationOptions.size();
        OperationOptionButton.label = OperationOptions[selectedOperationIndex].c_str(); 
    }

    if (OperationNextButton.IsClicked()) {

        selectedOperationIndex = (selectedOperationIndex + 1) % OperationOptions.size();
        OperationOptionButton.label = OperationOptions[selectedOperationIndex].c_str(); 
    }

    if(OperationOptions[selectedOperationIndex] == "INSERT") currentOperation = Operation::Insert;
    if(OperationOptions[selectedOperationIndex] == "CREATE") currentOperation = Operation::Create;
    if(OperationOptions[selectedOperationIndex] == "SEARCH") currentOperation = Operation::Search;
    if(OperationOptions[selectedOperationIndex] == "UPDATE") currentOperation = Operation::Update;
    if(OperationOptions[selectedOperationIndex] == "DELETE") currentOperation = Operation::Delete;

    //INPUT Event

    if (InputPrevButton.IsClicked()) {
        selectedInputIndex = (selectedInputIndex - 1 + InputOptions.size()) % InputOptions.size();
        if(currentOperation != Operation::Insert && currentOperation != Operation::Create){
            if(selectedInputIndex == 2) selectedInputIndex = 1;
            
        }
        InputOptionButton.label = InputOptions[selectedInputIndex].c_str(); 
    }

    if (InputNextButton.IsClicked()) {
        selectedInputIndex = (selectedInputIndex + 1) % InputOptions.size();
        if(currentOperation != Operation::Insert && currentOperation != Operation::Create){
            if(selectedInputIndex == 2) selectedInputIndex = 0;
        }
         InputOptionButton.label = InputOptions[selectedInputIndex].c_str(); 
    }
    
\

    if (InputOptions[selectedInputIndex] == "KEYBOARD") currentInput = InputType::Keyboard;
    if (InputOptions[selectedInputIndex] == "RANDOM") currentInput = InputType::Random;
    if (InputOptions[selectedInputIndex] == "FILE") currentInput = InputType::File;
}

std::mt19937 rng(std::random_device{}());
void Page::RANDOM_INPUT(){
    if (InputOptionButton.IsClicked()) {
                std::uniform_int_distribution<int> dist(0, 999); // Giới hạn số từ 0-999
                textbox.inputText = to_string(dist(rng)); // Lấy số ngẫu nhiên
    }
}
void Page::KEYBOARD_INPUT(){
     textbox.HandleInput(8);
}
void Page::FILE_INPUT(){
    if(IsFileDropped()){
        FilePathList droppedFiles = LoadDroppedFiles();
        TextCopy(filePath,droppedFiles.paths[0]);
        ifstream fin(filePath);
        if(!fin.is_open()) cout << "Cannot open dropped file";
        else{
            textbox.resetTextbox();
            int val;
                while (fin >> val) {
                textbox.nums.push_back(val);
            }       
            fin.close();
        }
        UnloadDroppedFiles(droppedFiles); 
    }
}

void Page::handleInput(){
     switch (currentOperation) {
        case Operation::Insert:
        case Operation::Create:

            switch (currentInput) {
            case InputType::Random:
                RANDOM_INPUT();
            break;
            case InputType::Keyboard:
                KEYBOARD_INPUT();
                break;
            case InputType::File:
               FILE_INPUT();
                break;
            default:
                break;
            }

        case Operation::Search:
        case Operation::Delete:
            switch (currentInput) {
            case InputType::Random:
                RANDOM_INPUT();
            break;
            case InputType::Keyboard:
                KEYBOARD_INPUT();
                break;
            default:
                break;
            }

        case Operation::Update:
            switch (currentInput) {
                case InputType::Random:
                    if (InputOptionButton.IsClicked()) {
                        std::uniform_int_distribution<int> dist(0, 999); 
                        newTextBox.inputText = to_string(dist(rng)); 
                        oldTextBox.inputText = to_string(dist(rng)); 
                    }
                    break;
                case InputType::Keyboard:
                    newTextBox.HandleInput(4);
                    oldTextBox.HandleInput(4);
                    break;
            }
            break;
    }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if(currentOperation == Operation::Update){
                if(oldTextBox.active && !oldTextBox.inputText.empty()){
                    oldTextBox.inputText.pop_back();
                }
                if(newTextBox.active && !newTextBox.inputText.empty()){
                    newTextBox.inputText.pop_back();
                }
            }
            else {
                if (!textbox.inputText.empty()) textbox.inputText.pop_back();
            }
        }

        if ((Ok.IsClicked() || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))) {
            if(currentOperation == Operation::Update){
                if(!oldTextBox.inputText.empty() && !newTextBox.inputText.empty()){
                    int val = stoi(oldTextBox.inputText);
                    textbox.nums.push_back(val);
                    val = stoi(newTextBox.inputText);
                    textbox.nums.push_back(val);
                    //textbox.nums[0] is old value, [1] is new
                    oldTextBox.inputText = "";
                    newTextBox.inputText = "";
                }
            }
            else{
                if(!textbox.inputText.empty()){
                    int val;
                    stringstream ss(textbox.inputText);
                    while(ss >> val){
                        textbox.nums.push_back(val);
                    }
                    // TraceLog(LOG_INFO, textbox.inputText.c_str());
                    textbox.inputText = "";
                }
            }

        }    
}

