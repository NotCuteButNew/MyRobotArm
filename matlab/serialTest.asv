% 定义角度数据和当前发送行号
clear; clc;
global cur_row data;
load('degData.mat');
[n_rows, n_cols] = size(data);
cur_row = 1;

% 创建Serial对象，并绑定回调函数
s = serialport('COM3',115200);
configureTerminator(s, "CR/LF");
configureCallback(s,"terminator",@myCallback); % 配置回调函数

% 打开串口通信
fopen(s);

% 不断读取Serial对象，直到所有数据全部发送
while cur_row <= n_rows
    % 如果有数据可读，则读取数据并触发回调函数
    if s.BytesAvailable > 0
        myCallback(s);
    end

    % 等待一小段时间（例如10毫秒），防止过度占用CPU资源
    %pause(0.01);

end

% 发送完成后关闭串口
fclose(s);

% 删除Serial对象，释放资源
delete(s);

% 定义回调函数，每次Serial对象接收到数据时就会调用该函数
function myCallback(obj)
global cur_row data;
msg=readline(obj);
disp(msg);
% 如果收到特定指令，则发送一行角度数据
if strcmp(msg, "INFO: ---waiting for the next command---")
    % 构造要发送的数据字符串
    data_str = sprintf("%.2f %.2f %.2f\r", data(cur_row, :));

    % 发送数据到串口中
    fprintf(obj, data_str);

    % 将当前发送行号加1，准备发送下一行数据
    
    cur_row = cur_row + 1;
    %disp(cur_row);
end
end
