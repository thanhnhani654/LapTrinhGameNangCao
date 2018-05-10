# LapTrinhGameNangCao
2 ban Client va Server Cua Game.
Roll Back Update hoan thanh phan nua:
Client gửi dữ liệu từng lần update lên server => server lưu các lần update vào danh sách, mỗi lần trước khi update Server sẽ tìm snapshoot gần với lần update client gửi lên nhất rồi tiến hành reupdate toàn bộ từ snapshoot đó lên.

Cách để test trên nhiều máy khác nhau:
1. Vào file server.cpp tìm đoạn code nào có vẻ là 1 địa chỉ ip. đổi lại thành ip máy server
2. Vào file client.h ngay chô szServer đổi lại thành ip của máy server.

Lưu ý: Nếu chạy file exe trong folder debug thì phải vào TaskManager tắt mới được. Do là tắt bình thường chỉ tắt được 1 Thread. Để từ từ làm cái tắt 2 thread
