# LapTrinhGameNangCao
2 ban Client va Server Cua Game.
Roll Back Update hoan thanh phan nua:
Client gửi dữ liệu từng lần update lên server => server lưu các lần update vào danh sách, mỗi lần trước khi update Server sẽ tìm snapshoot gần với lần update client gửi lên nhất rồi tiến hành reupdate toàn bộ từ snapshoot đó lên.
Chưa làm xong phần server gửi dữ liệu lại cho client kiểm tra.
