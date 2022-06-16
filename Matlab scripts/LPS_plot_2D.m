clear;
close all;

fig_handle = figure("Name", "Brightness distribution");
set(fig_handle,'WindowKeyPressFcn',@keyPressCallback);
global string keyPressed;
keyPressed = [];

data_horizontal = [];
data_vertical = [];

cog_horizontal_array = [];
cog_vertical_array = [];

cog_vertical = 0;
cog_horizontal = 0;

device = serialport("/dev/ttyACM0",115200);
fprintf("Serial port opened\n");

%skip first frame that may be corrupted
ASCII_data = readline(device);
ASCII_data = readline(device);
ASCII_data = readline(device);
ASCII_data = readline(device);

ASCII_data = readline(device);
eval(ASCII_data);
ASCII_data = readline(device);
eval(ASCII_data);
ASCII_data = readline(device);
eval(ASCII_data);
ASCII_data = readline(device);
eval(ASCII_data);

while (1)
    % wait until there is data avilable on serial port
    ASCII_data = readline(device);
    eval(ASCII_data);
    ASCII_data = readline(device);
    eval(ASCII_data);

    cog_vertical_array = [cog_vertical_array, cog_vertical];
    cog_horizontal_array = [cog_horizontal_array, cog_horizontal];
   
    fprintf("n = %d  ", length(cog_horizontal_array));
    fprintf("cog_v = %.4f", cog_vertical);
    fprintf("  cog_h = %.4f", cog_horizontal);
    fprintf("\n");

    plot(cog_horizontal_array, cog_vertical_array);
    grid on;
    %xlabel("Position of laser dot");
    xlim([0, 320]);
    ylim([0, 240]);

    if (length(cog_horizontal_array) == 30)
        save("test_y=-3.0mm_rgb565_320x240.mat");
        close(fig_handle);
        fprintf("Stopping...\n");
        device = [];
        fprintf("Serial port closed\n");
        return;
    end

    % if space was presses, take screenshot
    if (strcmp(keyPressed, "space"))
        keyPressed = [];
        saveas(fig_handle, "plot_" + datestr(now, 'dd.mm.yy-HH:MM:SS') + ".png");
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
