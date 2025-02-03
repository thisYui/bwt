# MyProject - Burrows-Wheeler Transform
Dồ án này thuộc vào học phần Cấu trúc dữu liệu và Giải thuật của trường Đại học Khoa học Tự nhiên, ĐHQG-TPHCM.

## Description
- Đồ án này tìm hiểu về cách chuyển đổi một chuỗi thành chuỗi đã mã hóa bằng BWT (Burrows-Wheeler Transform) và ngược lại.
- Tìm kiếm chuỗi pattern trong một đoạn văn bản.
- Nén và giải nén file đơn giải bằng thuật toán MTF (Move-to-Front) và RLE (Run-Length Encoding) kết hợp với BWT.
- Đồ án này là đồ án cá nhân được thực hiện bởi Nguyễn Quang Duy, sinh viên khóa 2023 thuộc Trường Đại học Khoa học Tự nhiên, ĐHQG-TPHCM.

## Features
- Feature 1: Chuyển đổi chuỗi thành chuỗi đã mã hóa bằng BWT và ngược lại.
- Feature 2: Tìm kiếm chuỗi pattern trong một đoạn văn bản.
- Feature 3: Nén và giải nén file đơn giản bằng thuật toán MTF và RLE kết hợp với BWT.

## Requirements
- Operating System: Linux / macOS / Windows
- Programming Language: C++11 hoặc bất kì phiên bản tương thích nào khác.
- Dependencies: Được code bằng Clion.

## Attention
- Đây là một chương trình Command Line Interface (CLI) nên cần phải chạy trên Command Prompt hoặc PowerShell.
- Cần phải biên dịch các file .cpp thành file main.exe trước khi sử dụng.
- Chú ý việc ghi đường dẫn khi sử dụng command line.

## Operating
### Bước 1: Clone dự án về máy của bạn.
### Bước 2: Biên dịch file thành file.exe
- Sử dụng Command Prompt hay PowerShell để biên dịch các file .cpp thành file .exe 
- Di chuyển đến thư mục chứa folder bằng lệnh 'cd'. Ví dụ : cd "D:\bwt" nếu như thư mục chứa file .cpp là D:\bwt, sau đó sử dụng lệnh g++ để biên dịch file .cpp thành file .exe

 g++ -o main.exe *.cpp
- Sau khi biên dịch thành công sẽ xuất hiện file main.exe nằm trong thư mục bwt/.
- Vì chứa các file trên bằng folder nên phải chú ý việc ghi đường dẫn khi dùng command line
- File input.txt trong thư mục command thì phải ghi đường dẫn là command\input.txt

### Bước 3: Sử dụng  main.exe
Chuyển đổi chuỗi thành chuỗi đã mã hóa bằng bwt
 .\main.exe -c command\input.txt output.txt --bwt
- input.txt : file chứa chuỗi cần mã hóa 
- output.txt : file chứa chuỗi đã mã hóa 

Chuyển đổi chuỗi dạng bwt thành chuỗi gốc
 .\main.exe -c output.txt restore.txt
- output.txt : file chứa chuỗi đã mã hóa sử dụng lại từ bước trên
- restore.txt : file chứa chuỗi gốc được phục hồi có thể so sánh với file input.txt

Tìm kiếm chuỗi pattern trong paragraph
.\main.exe -p command\paragraph.txt command\patterns.txt index.txt
- paragraph.txt : file chứa đoạn văn
- patterns.txt : file chứa các chuỗi cần tìm kiếm
- index.txt : file chứa vị trí xuất hiện của các chuỗi cần tìm kiếm trong đoạn văn

Nén file

.\main.exe -z command\log.txt zlog.txt
hoặc .\main.exe -z command\log.txt zlog.bin
- log.txt : file chứa dữ liệu cần nén
- zlog.txt hoặc zlog.bin: file chứa dữ liệu đã nén

Giải nén file

.\main.exe -z zlog.txt relog.txt --u
nếu nén thành bin: .\main.exe -z zlog.bin relog.txt --u
- zlog.txt hoặc zlog.bin : file chứa dữ liệu đã nén
- relog.txt : file chứa dữ liệu đã giải nén
