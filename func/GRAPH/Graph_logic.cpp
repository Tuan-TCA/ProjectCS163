#include "Graph.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include "Variables.h"
#include <algorithm>
#include "ControlAnimation.h"
#include <unordered_set>

using namespace std;

float distance(Vector2 pos1, Vector2 pos2){
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}
void Graph::init(){
    Page::init();
    MSTbutton =  Button(5, screenHeight / 2 - screenHeight*0.63f * 0.5f + 5, screenWidth*0.24f - 10, screenHeight*0.63f * 0.15f, "MST", MyColor1, Fade(MyColor1, 0.8f), WHITE); 
    added = false;
    minDistance = 100.0f;
    got1stV = false;
    isAnimating = false;
}

void Graph::draw(){
    Page::draw();


    for(auto& e: edge){
        // cout << "edge draw" << endl;
        e.Draw();
    }
    for(auto& v: vertex){
        v.Draw();
    }

    float pseudocodeX = screenWidth * 0.01f;
    float pseudocodeY = screenHeight * 0.5f;
    float lineHeight = 20.0f;
    Color highlightColor = YELLOW;
     if(isAnimating){
    for (size_t i = 0; i < pseudocode.size(); ++i) {
        Color currentColor = highlightColor;
        if(!StepQueue.empty() && find(StepQueue[currentQueueIndex].begin(), StepQueue[currentQueueIndex].end(), i) != StepQueue[currentQueueIndex].end()){
            currentColor.a = 255;
        }
        else{
            currentColor.a = 0;
        }
        
        // DrawText(pseudocode[i].c_str(), pseudocodeX, pseudocodeY + i * lineHeight, 20, currentColor);
        float textWidth = MeasureText(pseudocode[i].c_str(), 14);
        DrawRectangleRounded(Rectangle{pseudocodeX - 5, pseudocodeY + i * lineHeight - 5, screenWidth*0.23f,  lineHeight}, 1, 5, currentColor);
        DrawTextEx(FONT, pseudocode[i].c_str(), {pseudocodeX, pseudocodeY + i * lineHeight} , 10, 2, MyColor4);
        
    }
    }

}


void Graph::event(){
    Page::event();

    handleInput();
   if(!added){
        addFromMatrix(); 
       added = true;
   }

     
    for(auto& v: vertex){
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            if(CheckCollisionPointCircle(GetMousePosition(), v.position, v.radius) || v.isPressing){
                v.position = GetMousePosition();
                
                v.isPressing = true;
            }
        }else{
            v.isPressing = false;
        }
    }
   
    for(auto& e: edge){
        
        if(!e.isAnimating || (e.isAnimating && e.endEdge != e.start->position && e.endEdge != e.end->position)) e.Update(0);
        
    }
    handleCollision();
    
    if (back1.IsClicked() || back2.IsClicked()) {
        
        isPlaying = false;

        if (!arrayQueue.empty() && currentQueueIndex < arrayQueue.size()) {
            vector<Drawable*> current = arrayQueue[currentQueueIndex];
            for (auto& elem : current) {
                elem->reset();
            }
            currentQueueIndex--;
            if (currentQueueIndex < 0) currentQueueIndex = 0;
            if (currentQueueIndex < arrayQueue.size()) {
                vector<Drawable*> prev = arrayQueue[currentQueueIndex];
                for (auto& elem : prev) {
                    elem->reset();
                    elem->isAnimating = true; 
                }
            }
        }

    } 
    else if (next1.IsClicked() || next2.IsClicked()) {

        isPlaying = false; 

        if (!arrayQueue.empty() && currentQueueIndex < arrayQueue.size()) {
            vector<Drawable*> current = arrayQueue[currentQueueIndex];
            for (auto& elem : current) {
                elem->SetColor(ORANGE);
                elem->isAnimating = false;
            }
            currentQueueIndex++;
            if (currentQueueIndex > arrayQueue.size() - 1) currentQueueIndex = arrayQueue.size() - 1;
            if (currentQueueIndex < arrayQueue.size()) {
                vector<Drawable*> next = arrayQueue[currentQueueIndex];
                for (auto& elem : next) {
                    elem->SetColor(ORANGE);
                    elem->isAnimating = false; // ready to be animating
                }
            }
        }

    }


   handleBFS();
    // cout << currentIndex << endl
   
}

void Graph::startAnimation(float duration){
    this->duration = duration;
    currentQueueIndex = 0;
}





void Graph::addFromMatrix(){
     srand(static_cast<unsigned int>(time(0) + rand()));
    int n = matrix.size();
    //random vertices in workplace 
    int minX = screenWidth * 0.3f, maxX = screenWidth - 100;  
    int minY = screenHeight * 0.2f, maxY = screenHeight * 0.8f;
    vertex.clear();
    vertex.resize(n);
    for(int i = 0; i < n; i++){
        Vertex vertex_;
        vertex_.value = i;
        float newPosX, newPosY;
        bool validPosition = false;
        while(!validPosition){
            newPosX = rand() % (maxX - minX + 1) + minX;
            newPosY = rand() %  (maxY - minY + 1) + minY;
            validPosition = true;
            for(int j = 0; j <= i;j++){
                if(distance({newPosX, newPosY}, vertex[j].position) < minDistance){
                    validPosition = false;
                    break;
                }
            }
        }
        vertex_.position.x =  newPosX;
        vertex_.position.y =  newPosY;    
         cout << "Vertex " << i << " position: (" << vertex_.position.x << ", " << vertex_.position.y << ")" << endl;
        vertex[i] = vertex_;
        
    }

    edge.clear();

    for(int i = 0; i < matrix.size(); i++){
        for(int j = i; j < matrix.size(); j++){ // bat dau tu 0
            if(matrix[i][j]){
                edge.push_back(Edge(&vertex[i], &vertex[j], matrix[i][j]));
            }
        }
    }
    cout << "EDGE: " << endl;
    for(auto& e: edge){
        e.print();
    }
}
void Graph::handleInput(){
    
    switch (currentInput) {
            case InputType::Random:
            if (InputOptionButton.IsClicked()) {
                 // Lấy số ngẫu nhiên
            }
            break;
            case InputType::Keyboard:
                textbox.HandleInput(7); 
                break;
            case InputType::File:
                if(IsFileDropped()){
                    
                    FilePathList droppedFiles = LoadDroppedFiles();
                    TextCopy(filePath,droppedFiles.paths[0]);
                    ifstream fin(filePath);
                    if(!fin.is_open()) cout << "Cannot open dropped file";
                    else{
                        cout << "File path: " << filePath << endl;
                        reset(); // reset graph;
                        textbox.resetTextbox();
                        matrix.clear();
                        int n;
                        fin >> n;
                        matrix.resize(n, vector<int>(n));
                        for(int i = 0; i < n; i ++){
                            for(int j = 0; j < n; j++){
                                fin >> matrix[i][j];
                                cout << "matrix[" << i << "][" << j << "] = " << matrix[i][j] << endl;
                            }
                        }     
                        fin.close();
                    }
                    UnloadDroppedFiles(droppedFiles); 
                }
                break;
            default:
                break;
        }

}

void Graph::reset(){
    added = false;
    vertex.clear();
    edge.clear();
    matrix.clear();
    arrayQueue.clear();
    isAnimating  = false;
    got1stV = false;
    bfsCalled = false;
    clickedV = nullptr;
    currentQueueIndex = 0;
    currentStep = 0;
}

void Graph::update(){
    float deltaTime = GetFrameTime();


}

void Graph::handleCollision(){
    
    for(int i = 0; i < vertex.size(); i++){
        for(int j = 0; j < vertex.size(); j++){
            float dis = distance(vertex[i].position, vertex[j].position);
            if( dis && dis  < minDistance){
                Vector2 delta = vertex[i].position - vertex[j].position;
                Vector2 rate = delta / dis;
                delta = rate * (minDistance - dis);
                if(vertex[i].isPressing){
                    vertex[j].position = vertex[j].position - delta;
                }
            }
        }
    }

}