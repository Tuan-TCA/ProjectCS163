#include "LinkedList.h"
#include<bits/stdc++.h>
#include <raylib.h>
using namespace std;


Node::Node(int key, Node* nextnode) {
    val = key;
    next = nextnode;
}

void Node::DrawRim(Vector2 center, float rad, Color colorRim){
    // BeginDrawing();

    //vẽ hình node
    for(int i = 0;  i < 15; i++){
        DrawCircleLines(center.x, center.y, rad + i, colorRim);
    }
    // EndDrawing();
}

void Node::Draw(Vector2 pos, float rad, Color colorNode, Color colorText) {
    // Vẽ hình tròn đại diện cho node
    DrawCircleV(pos, rad, colorNode);

    // Vẽ viền
    DrawCircleLines(pos.x, pos.y, rad, BLACK);
    
    // Vẽ giá trị của node
    std::string text = std::to_string(val);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 2);
    DrawText(text.c_str(), pos.x - textSize.x / 2, pos.y - textSize.y / 2, 20, colorText);
}

LinkedList::LinkedList() {
    head = nullptr;
    workplace = {400,300,600,600};
    position = {workplace.x*1.1f, workplace.y*1.1f};
    colorNode = LIGHTGRAY;
    colorText = BLACK;
    dis = 100;
    rad = 30;
    isInserting = false;
}

void LinkedList::DrawLL() {
    if (!head) {
        //std::cout << "List is empty!" << std::endl;
        return;
    }
    Node* cur = head;
    Vector2 curPos = position;
    while (cur) {  // Duyệt đến hết danh sách
        cur->Draw(curPos, rad, colorNode, colorText);
        if (cur->next) {  // Vẽ đường nối giữa các node
            DrawRectangle(curPos.x + rad,curPos.y, 2.0*rad, 4.0, BLACK);
        }
        curPos.x += dis; // Dịch chuyển vị trí để vẽ node tiếp theo
        cur = cur->next;
    }
    //std::cout << "Draw complete!" << std::endl;
}


void LinkedList::DrawInsert(int key) {
    // Đánh dấu rằng chúng ta đang thực hiện hiệu ứng chèn
    isInserting = true;

    // Nếu danh sách rỗng, tạo node đầu tiên với hiệu ứng phóng to
    if (!head) {
        head = new Node(key, nullptr);
        // Hiệu ứng: vẽ node với bán kính tăng dần
        // for (float r = rad; r <= rad * 2; r += 5) {
        //     BeginDrawing();
        //     //ClearBackground(RAYWHITE);
        //     DrawLL();
        //     head->Draw(position, r, YELLOW, BLACK);
        //     EndDrawing();
        //     std::this_thread::sleep_for(std::chrono::milliseconds(50));
        // }
        isInserting = false;
        return;
    }

    // Nếu danh sách không rỗng, tìm vị trí chèn ở cuối
    Node* cur = head;
    Vector2 curPos = position;
    while (cur->next) {
        // Vẽ hiệu ứng highlight cho từng node trên đường đi
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        cur->DrawRim(curPos, rad, YELLOW);
        DrawLL(); // Cập nhật lại màn hình để thấy hiệu ứng
        cout<<"@";
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        cur = cur->next;
        curPos.x += dis;
    }
    // Thêm node mới vào cuối danh sách
    cur->next = new Node(key, nullptr);
    curPos.x +=dis;
    // Hiệu ứng phóng to cho node mới
    for (float r = rad; r <= rad * 2; r += 5) {
        BeginDrawing();
        //ClearBackground(RAYWHITE);
        DrawLL();
        cur->next->Draw(curPos, r, YELLOW, BLACK);
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    isInserting = false;
}