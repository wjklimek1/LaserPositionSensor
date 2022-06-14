clear;
close all;

fig_handle = figure("Name", "Brightness distribution");
set(fig_handle,'WindowKeyPressFcn',@keyPressCallback);
global string keyPressed;
keyPressed = [];

data_horizontal = [];
data_vertical = [];

device = serialport("/dev/ttyACM0",115200);
fprintf("Serial port opened\n");

%skip first frame that may be corrupted
ASCII_data = readline(device);

while (1)
    % wait until there is data avilable on serial port
    ASCII_data = readline(device);
    eval(ASCII_data);
    
    for i = 1:length(data_horizontal)
        if isnan(data_horizontal(i))
            data_horizontal(i:length(data_horizontal)) = [];
            break;
        end
    end
    for i = 1:length(data_vertical)
        if isnan(data_vertical(i))
            data_vertical(i:length(data_vertical)) = [];
            break;
        end
    end
    
    subplot(2,1,1);
    plot(data_horizontal);
    grid on;
    xlim([1, length(data_horizontal)]);
    xlabel("Brightness sums of pixel rows");
    
    subplot(2,1,2);
    plot(data_vertical);
    grid on;
    xlim([1, length(data_vertical)]);
    xlabel("Brightness sums of pixel columns");

    % if space was presses, take screenshot
    if (strcmp(keyPressed, "space"))
        keyPressed = [];
        saveas(fig_handle, "distribution_" + datestr(now, 'dd.mm.yy-HH:MM:SS') + ".png");
    end
    % if esc key was presses, take screenshot
    if (strcmp(keyPressed, "escape"))
        keyPressed = [];
        break;
    end
end

close(fig_handle);
fprintf("Stopping...\n");
device = [];
fprintf("Serial port closed\n");


% key pressed callback
function keyPressCallback(source,eventdata)
   global keyPressed;
   keyPressed = eventdata.Key;
end
