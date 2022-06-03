clear;

stop_condition = 0;
fig_handle = figure("Name", "Brightness distribution");

data_horizontal = [];
data_vertical = [];

device = serialport("/dev/ttyACM0",115200);
fprintf("Serial port opened\n");

while 1

    % wait until there is data avilable on serial port
    ASCII_data = readline(device);
    eval(ASCII_data);
    
    for i = 1:length(data_horizontal)
        if isnan(data_horizontal(i))
            data_horizontal(i:length(data_horizontal)) = [];
            break;
        end
    end
    
    subplot(2,1,1);
    plot(data_horizontal);
    xlim([1, length(data_horizontal)]);
    
    subplot(2,1,2);
    plot(data_vertical);
    xlim([1, length(data_vertical)]);

    isKeyPressed = ~isempty(get(fig_handle,'CurrentCharacter'));
    if isKeyPressed
        break;
    end
end

fprintf("Stopping...\n");
device = [];
fprintf("Serial port closed\n");