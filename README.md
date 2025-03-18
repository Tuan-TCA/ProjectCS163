### Hãy thoải mái hỏi trong group ngay khi có thắc mắc

## Cách lấy repo này về máy local để code
- Tạo thư mục mới
- MỞ thư mục đó trong VScode
- Nhập các dòng sau trong terminal

git init    #khởi tạo git      

git remote add origin https://github.com/Tuan-TCA/ProjectCS163  #liên kết với repos chung của nhóm

git pull origin main    #kéo repos về máy local

git checkout -b 'tên branch mới'    #tạo branch mới để làm việc (đặt theo tên của mình)

## Làm việc trên branch mới này.
### Sau khi code, thực hiện thay đổi ..v.v. muốn commit:

git add .   #thêm các sửa đổi vào vùng stage

git commit -m "tin nhắn commit"     #commit các sửa đổi kèm lời nhắn tại local

git push -u origin 'tên branh mới'  #đẩy các commit lên remote branch

//Đọc thêm các lệnh về git hoặc học cách sử dụng GitHub Destop

## Hướng dẫn tạo Pull Request trên GitHub

Sau khi đã push nhánh lên remote repository, hãy làm theo các bước sau để tạo Pull Request (PR):

1. Truy cập GitHub
- Mở trình duyệt và truy cập repository của bạn:  
  [https://github.com/Tuan-TCA/ProjectCS163](https://github.com/Tuan-TCA/ProjectCS163)
- Nếu nhánh vừa push có thể tạo PR, bạn sẽ thấy thông báo **"Compare & pull request"** gần nhánh đó. Bấm vào đó.
- Nếu không thấy, vào tab **Pull requests** và bấm nút **New pull request**.


2. Chọn nhánh cần merge
- Ở trang tạo PR, chọn:
  - **Base branch**: Nhánh mà bạn muốn merge vào (**nhánh `main`**).
  - **Compare branch**: Nhánh chứa thay đổi của bạn.


3. Viết mô tả cho Pull Request
- Nhập **tiêu đề (Title)** mô tả ngắn gọn về nội dung PR.
- Thêm mô tả chi tiết về thay đổi trong phần **Description** (**bắt buộc thêm ít nhất 1 ảnh**).


4. Gửi Pull Request
- Bấm **Create pull request** để gửi PR lên repository.

---
💡Sau khi PR được duyệt, bạn có thể merge PR vào nhánh chính và xóa nhánh không cần thiết.