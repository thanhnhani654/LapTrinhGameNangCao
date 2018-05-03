# LapTrinhGameNangCao
2 ban Client va Server Cua Game.
Roll Back Update hoan thanh phan nua:
Client gửi dữ liệu từng lần update lên server => server lưu các lần update vào danh sách, mỗi lần trước khi update Server sẽ tìm snapshoot gần với lần update client gửi lên nhất rồi tiến hành reupdate toàn bộ từ snapshoot đó lên.
Chưa làm xong phần server gửi dữ liệu lại cho client kiểm tra.
Lưu ý:
+ Dữ liệu Client gửi lên server bị dư quá nhiều lần Update không có sự kiện để cập nhật
+ class Server trên bản Server chưa chuyển thành SingleTon
+ Việc update tọa của tank nằm trong phần update game. Nghĩa là FPS của server là 60 và speed của tank là 1 thì Tank sẽ di chuyển được 60 pixel/s. Ảnh hưởng lớn đến sự khác biệt của Client-Server. Tạm thời chưa có cách giải quyết (Đã thử lấy position += velocity * (DeltaTime / (1000.0f / 60.0f); Nhưng bản game chạy bị giật)
+ Ở Server có cái lỗi Ram tăng vô tội dạ là do việc tạo SnapShoot tạo con trỏ liên tục mà không xóa
