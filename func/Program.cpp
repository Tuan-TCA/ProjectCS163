#include "Program.h"
#include <raylib.h>
#include <iostream>

Program::Program () {
    InitWindow(screenWidth, screenHeight, "Visualization App - CS 163 - Group 12");
    SetTargetFPS(60);
    mode = MODE::MENU;
    menu.init();
}


void Program::run() {
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        event();
        EndDrawing();
    }
    CloseWindow();
}

void Program::event() {
    if(mode == MODE::MENU) {
        int x;
        x = menu.modeRetrieve();
        if(x == 1) {
            mode = MODE::LL;
            // std::cout<<"Linked list"; 
            page.init();
        }
        else if(x == 2) {
            mode = MODE::HASHTB;
            page.init();
            
        }
        else if(x == 3) {
            mode = MODE::AVL;
            page.init();
            
        }
        else if(x == 4) {
            mode = MODE::GRAPH;
            page.init();
            
        }
    }

    
    else if(mode == MODE::LL) {
        // Linked List
        page.event();
    }
    if(mode == MODE::HASHTB) {
        // Hash Table
        page.event();
    }
    if(mode == MODE::AVL) {
        // AVL Tree
        page.event();
    }
    if(mode == MODE::GRAPH) {
        // Graph   
        page.event();
    }
}

void Program::draw() {
    if (mode == MODE::MENU) {
        menu.draw();
    } 
    if (mode == MODE::LL) {
        //LL.draw();
        page.draw();
    }
     
    if (mode == MODE::HASHTB) {
        //LL.draw();
        page.draw();
    }
    if (mode == MODE::AVL) {
        //LL.draw();
        page.draw();
    }
    if (mode == MODE::GRAPH) {
        //LL.draw();
        page.draw();
    }
}



