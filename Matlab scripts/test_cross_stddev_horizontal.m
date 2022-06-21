clear;
close all;

cog_horizontal_plot = [];
cog_vertical_plot = [];
std_dev_horizontal = [];
std_dev_vertical = [];

figure('Name', "standard deviation horizontal");

plot_test("rgb888", '320x240', '-r.');
plot_test("rgb888", '160x120', '-g.');
plot_test("rgb565", '320x240', '-b.');
plot_test("rgb565", '160x120', '-k.');

grid on;
grid minor;
xlabel("horizontal dot position");
ylabel("standard deviation");
xlim([-3.1, 3.1]);

legend("RGB888 320x240", "RGB888 160x120", "RGB565 320x240", "RGB565 160x120");

function plot_test(color_depth, resolution, color)

    locations_tab = ["3.0", "2.5", "2.0", "1.5", "1.0", "0.5", "0.0", "-0.5", "-1.0", "-1.5", "-2.0", "-2.5", "-3.0"];
   
    for i = 1:length(locations_tab)
        filename = append("test_y=", locations_tab(i), "mm_", color_depth, "_", resolution, ".mat");
        plot_stddev(filename, locations_tab(i), color);
    end
end

function plot_stddev(filename, position, color)
    load(filename);
    close(fig_handle);

    if color == "-b." || color == "-k."
        plot(str2double(position), std(cog_horizontal_array)*2, color,'MarkerSize',12);
    end

    if color == "-r." || color == "-g."
        plot(str2double(position), std(cog_horizontal_array), color,'MarkerSize',12);
    end

    hold on;
end