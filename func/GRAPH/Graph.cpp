#include "Graph.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <random>
#include <sstream>
using namespace std;

float distance(Vector2 pos1, Vector2 pos2){
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

void Graph::init(){
    Page::init();

    OperationOptions = {"CREATE", "ALGORITHM"};
    selectedAlgorithmIndex = 0;
    currentALgorithm = Algorithm::BFS;
    AlgorithmOptionButton = Button(screenWidth * 0.24f * 0.15f,screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 , screenWidth*0.24f * 0.7f, screenHeight*0.63f * 0.15f, AlgorithmOptions[selectedAlgorithmIndex].c_str(), WHITE, LIGHTGRAY, MyColor5);
    AlgorithmPrevButton = Button(5,screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 ,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f, "<", WHITE, LIGHTGRAY, MyColor5);
    AlgorithmNextButton = Button(screenWidth*0.24f * 0.85f + 5, screenHeight / 2 - screenHeight*0.63f * 0.35f + 10 ,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f, ">", WHITE, LIGHTGRAY, MyColor5);
    added = false;
    minDistance = 100.0f;
    got1stV = false;
    isAnimating = false;
}

void Graph::draw(){
    Page::draw();

    for(auto& e: edge){
        e.Draw();
    }
    for(auto& v: vertex){
        v.Draw();
    }

    //Algorithm
    if(currentOperation == Operation::Algorithm){
        AlgorithmOptionButton.Draw(LIGHTGRAY, WHITE);
        AlgorithmPrevButton.Draw(LIGHTGRAY, WHITE);
        AlgorithmNextButton.Draw(LIGHTGRAY, WHITE);
    }

    //code state

    Color highlightColor = Color{255, 222, 89, 255};
     if(isAnimating || currentOperation == Operation::Algorithm){
    for (size_t i = 0; i < pseudocode.size(); ++i) {
        Color currentColor = highlightColor;
        if(!StepQueue.empty() && find(StepQueue[currentQueueIndex].begin(), StepQueue[currentQueueIndex].end(), i) != StepQueue[currentQueueIndex].end()){
            currentColor.a = 255;
        }
        else{
            currentColor.a = 0;
        }
        DrawRectangleRounded(Rectangle{codeDisplayPLace.x, codeDisplayPLace.y + 10 + i * lineHeight - 5, codeDisplayPLace.width,  lineHeight}, 0, 5, currentColor);
        DrawTextEx(FONT, pseudocode[i].c_str(), {codeDisplayPLace.x + 5, codeDisplayPLace.y + 10 + i * lineHeight} , 10, 2, MyColor4);
        
    }
    }

}


void Graph::event(){
    Page::event();
    HandleInput();
   if(!added){
        addFromMatrix(); 
       added = true;
   }

    if( currentOperation == Operation::Create){
        addFromTextbox();
    }
    
    if(currentQueueIndex > arrayQueue.size()) isPlaying = false;
    //ALgorithm operation
    if(OperationOptions[selectedOperationIndex] == "ALGORITHM") currentOperation = Operation::Algorithm;

    if (AlgorithmPrevButton.IsClicked()) {
        resetAnimation();
        selectedAlgorithmIndex = (selectedAlgorithmIndex - 1 + AlgorithmOptions.size()) % AlgorithmOptions.size();
        AlgorithmOptionButton.label = AlgorithmOptions[selectedAlgorithmIndex].c_str(); 
    }

    if (AlgorithmNextButton.IsClicked()) {
        resetAnimation();
        selectedAlgorithmIndex = (selectedAlgorithmIndex + 1) % AlgorithmOptions.size();
        AlgorithmOptionButton.label = AlgorithmOptions[selectedAlgorithmIndex].c_str(); 
    }

    if(AlgorithmOptions[selectedAlgorithmIndex] == "BFS") currentALgorithm = Algorithm::BFS;
    if(AlgorithmOptions[selectedAlgorithmIndex] == "DFS") currentALgorithm = Algorithm::DFS;
    if(AlgorithmOptions[selectedAlgorithmIndex] == "MST") currentALgorithm = Algorithm::MST;
    if(AlgorithmOptions[selectedAlgorithmIndex] == "Connected\nComponents") currentALgorithm = Algorithm::CC;
    
    // if(currentALgorithm == Algorithm::BFS) cout << "bfs" << endl;
    // if(currentALgorithm == Algorithm::MST) cout << "mst" << endl;
    //vertex
    for(auto& v: vertex){
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            if(CheckCollisionPointCircle(GetMousePosition(), v.position, v.radius) || v.isPressing){
                v.position = GetMousePosition();
                // cout << v.position.x / screenWidth << " - " << v.position.y / screenHeight << endl;
                v.isPressing = true;
            }
        }
        else  if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
            if(CheckCollisionPointCircle(GetMousePosition(), v.position, v.radius) || v.isPressing){
                Vector2 mousePos = GetMousePosition();
                Vector2 delta = mousePos - v.position;
                v.isPressing = true;
                for(auto& v_ : vertex){
                    v_.position = v_.position + delta;
                }
                
            }
        
        }
        else v.isPressing = false;
        
    }

    for(auto& e: edge){
        
        if(!e.isAnimating || (e.isAnimating && e.endEdge != e.start->position && e.endEdge != e.end->position)) e.Update(0);
        
    }
    handleCollision();
    
    if (back1.IsClicked() || back2.IsClicked() || (!textbox.active && IsKeyPressed(KEY_LEFT))) {
        
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
    else if (next1.IsClicked() || next2.IsClicked() || (!textbox.active && IsKeyPressed(KEY_RIGHT))) {

        isPlaying = false; 

        if (!arrayQueue.empty() && currentQueueIndex < arrayQueue.size()) {
            vector<Drawable*> current = arrayQueue[currentQueueIndex];
            for (auto& elem : current) {
                elem->SetColor();
            }
            currentQueueIndex++;
            if (currentQueueIndex > arrayQueue.size() - 1) currentQueueIndex = arrayQueue.size() - 1;
            if (currentQueueIndex < arrayQueue.size()) {
                vector<Drawable*> next = arrayQueue[currentQueueIndex];
                for (auto& elem : next) {
                    elem->SetColor();
                }
            }
        }

    }

    if(backward1.IsClicked() || backward2.IsClicked() || (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_LEFT))){
        isPlaying = false;  
        if(!arrayQueue.empty() && currentQueueIndex < arrayQueue.size()){
            for(auto& s: arrayQueue){
                for(auto& elem: s){
                    elem->reset();
                }
            }
            currentQueueIndex = 0;
            for(auto& s: arrayQueue[0]){
                s->isAnimating = true;
            }
        }
    }
    else if (forward1.IsClicked() || forward2.IsClicked() || (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_RIGHT))){
        isPlaying = false;
        if(!arrayQueue.empty() && currentQueueIndex < arrayQueue.size()){
        for(auto& s: arrayQueue){
            for(auto& elem : s){
                elem->SetColor();
            }
        }    
        currentQueueIndex = arrayQueue.size() - 1;
        }
    }



    if(currentOperation == Operation::Algorithm && currentALgorithm == Algorithm::MST){
    Ok.label = "START";
    Ok.bounds = Rectangle{side.x * 1.2f + 5, side.y + screenHeight*0.63f * 0.3f + 15, screenWidth*0.24f - 10, screenHeight*0.63f * 0.15f};
    }
    else{
        Ok.label = "OK";
        Ok.bounds = Rectangle{ side.x + (side.x  + side.width) * 0.74f, side.y + screenHeight*0.63f * 0.3f + 15, 73, screenHeight*0.63f * 0.15f};
       
    }
   handleChoice();
    // cout << currentIndex << endl
   
}
void Graph::handleChoice(){
    if(currentOperation == Operation::Algorithm){
        // currentInput = InputType::Keyboard;
        switch (currentALgorithm)
        {
        case Algorithm::BFS:
            pseudocode = {
                "BFS (G, s)",
                " while !Q.empty // Q is a normal queue",
                "   for each neighbor v of u = Q.front, Q.pop",
                "       if v is unvisited, tree edge, Q.push(v)",
            };
            handleBFS();
            break;
        case Algorithm::MST:
            pseudocode = {
                "Sort E edges by increasing weight", //0
                "T = {}",
                "for (i = 0; i < edgeList.length; i++)", // 2
                "  if adding e = edgelist[i] does not form a cycle", //3
                "    add e to T", // 4
                "  else ignore e", // 5
                "MST = T",
            };
            handleMST();
            break;
        case Algorithm::DFS:
            pseudocode = {

            };
            break;
            case Algorithm::CC:
            pseudocode = {};
            handleCC();
            break;
        default:
           break;
        }
    }
    else{
        resetAnimation();
    }
}
void Graph::startAnimation(float duration){
    this->duration = duration;
    currentQueueIndex = 0;
}

void Graph::resetAnimation(){
    StepQueue.clear();
    arrayQueue.clear();
    isAnimating  = false;
    got1stV = false;
    AlgoCalled = false;
    clickedV = nullptr;
    currentQueueIndex = 0;
    currentStep = 0;
    for(auto& v: vertex){
        v.reset();
    }
    for(auto& e: edge){
        e.reset();
    }
}




void Graph::addFromMatrix(){
     srand(static_cast<unsigned int>(time(0) + rand()));
    int n = matrix.size();
    //random vertices in workplace 
    int minX = workplace.x, maxX = workplace.x + workplace.width;  
    int minY = workplace.y, maxY = workplace.y + workplace.height;
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

    if(currentALgorithm == Algorithm::MST) dsu.init(vertex.size()); // init dsu with n sets
    cout << "EDGE: " << endl;
    for(auto& e: edge){
        e.print();
    }
}

void Graph::addFromTextbox(){
    if(textbox.nums.size() > 0){
        
        reset();
        int n = textbox.nums[0];
        matrix.resize(n, vector<int>(n));
        int idx = 1;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                matrix[i][j] = textbox.nums[idx++];
            }
        }

        textbox.nums.clear();
        
    }
}



void Graph::RANDOM_INPUT() {
    std::mt19937 rng(std::random_device{}());
    
    switch (currentOperation) {

        case Operation::Create: {
            textbox.reset();
            std::uniform_int_distribution<int> vertexDist(5, 20); 
            int numVertices = vertexDist(rng);
           std::uniform_int_distribution<int> fullWeightDist(0, 10);
            std::uniform_real_distribution<double> zeroProbDist(0.0, 1.0);
            double probabilityOfZero = 0.6; // 60 % to get 0

            std::vector<std::vector<int>> adjMatrix(numVertices, std::vector<int>(numVertices, 0));
            std::vector<std::string> lines;
            std::stringstream ss;

            for (int i = 0; i < numVertices; ++i) {
                for (int j = i; j < numVertices; ++j) {
                    if (i == j) {
                        adjMatrix[i][j] = 0;
                    } else {
                        if (zeroProbDist(rng) < probabilityOfZero) {
                            adjMatrix[i][j] = 0;
                        } else {
                            adjMatrix[i][j] = fullWeightDist(rng);
                        }
                        adjMatrix[j][i] = adjMatrix[i][j];
                    }
                }
            }

        //    matrix = adjMatrix;
            ss << numVertices;
            lines.push_back(ss.str());
            ss.str(""); 

            for (int i = 0; i < numVertices; ++i) {
                for (int j = 0; j < numVertices; ++j) {
                    ss << adjMatrix[i][j] << (j == numVertices - 1 ? "" : " ");
                }
                lines.push_back(ss.str());
                ss.str(""); 
            }

            textbox.inputText = lines;
            textbox.SubIndex.clear();
            for (const auto& line : lines) {
                textbox.SubIndex.push_back(line.length()); 
            }
            textbox.currentIndex = 0;
            break;
        }
        default:
        // Page::handleInput();
            break;
    }
}
void Graph::FILE_INPUT(){
    switch (currentOperation)
    {
    case Operation::Insert:
        //Insert graph
        break;
    case Operation::Create:
        if(IsFileDropped()){        
        FilePathList droppedFiles = LoadDroppedFiles();
        TextCopy(filePath,droppedFiles.paths[0]);
        ifstream fin(filePath);
        if(!fin.is_open()) cout << "Cannot open dropped file";
        else{
            cout << "File path: " << filePath << endl;
            reset(); // reset graph;
            textbox.reset();
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
    Page::handleInput();
        break;
    }
    
}

void Graph::reset(){
    Page::reset();
    added = false;
    vertex.clear();
    edge.clear();
    matrix.clear();
    arrayQueue.clear();
    StepQueue.clear();
    isAnimating  = false;
    got1stV = false;
    AlgoCalled = false;
    clickedV = nullptr;
    currentQueueIndex = 0;
    currentStep = 0;
    selectedAlgorithmIndex = 0;
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
                else{
                    vertex[i].position = vertex[i].position + delta;
                }
            }
        }
    }

    //bound the position
    for(auto& s : vertex){
        if(s.position.x < workplace.x) s.position.x = workplace.x;
        if(s.position.y < workplace.y) s.position.y = workplace.y;
        if(s.position.x > workplace.x + workplace.width) s.position.x = workplace.x + workplace.width;
        if(s.position.y > workplace.y + workplace.height) s.position.y = workplace.y + workplace.height;
    }

}

void Graph::HandleInput(){
    if(currentOperation == Operation::Algorithm){
        textbox.HandleInput();
    }
}

void Graph::updateSide(){
    Page::updateSide();
    AlgorithmOptionButton.bounds =Rectangle{side.x + (side.x + side.width) * 0.15f, side.y + screenHeight*0.63f * 0.15f + 10 , screenWidth*0.24f * 0.7f, screenHeight*0.63f * 0.15f};
    AlgorithmPrevButton.bounds = Rectangle{side.x + 5,side.y + screenHeight*0.63f * 0.15f + 10 ,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f}; 
    AlgorithmNextButton.bounds = Rectangle{side.x + (side.x + side.width) * 0.85f + 5, side.y + screenHeight*0.63f * 0.15f + 10,  screenWidth*0.24f * 0.15f - 10, screenHeight*0.63f * 0.15f};;
}