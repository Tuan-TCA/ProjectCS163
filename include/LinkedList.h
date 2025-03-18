#pragma once
#include <string>
#include <math.h>
#include <iostream>
using namespace std;
#include <raylib.h>
#include "Variables.h"
class Node {
public:
    int val;
    Node* next;

    Node(int key, Node* nextnode);

    void Draw(Vector2 pos,float rad, Color colorNode, Color colorText);
    void DrawRim(Vector2 center, float rad,Color colorRim);
};


class LinkedList {
    public:
        Node* head;
        Rectangle workplace;
        Vector2 position;
        Color colorNode;
        Color colorText;
        float dis;
        float rad;
        float mytime=0.01;
        
        // Biến kiểm soát hiệu ứng chèn
        bool isInserting;
    
        LinkedList();
    
        ~LinkedList() {
            Node* current = head;
            head = nullptr;
            while (current) {
                Node* nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }
    
        // Vẽ danh sách liên kết
        void DrawLL();
    
        // Thêm node vào cuối danh sách (không có hiệu ứng)
        void Insert(int key);
    
        // Thực hiện chèn có hiệu ứng (animation)
        void DrawInsert(int key);
    };