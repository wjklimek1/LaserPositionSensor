clear;
close all;

cog_horizontal_plot = [];
cog_vertical_plot = [];
std_dev_horizontal = [];
std_dev_vertical = [];

fileID = fopen('stats.txt','w');
fprintf(fileID, "%s\n", "TEST RESULTS:");
fclose(fileID);

figure('Name', "RGB888, 320x240");
plot_test("rgb888", '320x240');
grid on;
grid minor;
xlim([0, 320]);
ylim([0, 240]);

figure('Name', "RGB565, 320x240");
plot_test("rgb565", '320x240');
grid on;
grid minor;
xlim([0, 320]);
ylim([0, 240]);

figure('Name', "RGB888, 160x120");
plot_test("rgb888", '160x120');
grid on;
grid minor;
xlim([0, 160]);
ylim([0, 120]);

figure('Name', "RGB565, 160x120");
plot_test("rgb565", '160x120');
grid on;
grid minor;
xlim([0, 160]);
ylim([0, 120]);

function plot_test(color_depth, resolution)

    locations_tab = ["3.0", "2.5", "2.0", "1.5", "1.0", "0.5", "0.0", "-0.5", "-1.0", "-1.5", "-2.0", "-2.5", "-3.0"];
   
    for i = 1:length(locations_tab)
        filename = append("test_x=", locations_tab(i), "mm_", color_depth, "_", resolution, ".mat");
        plot_position(filename);
    end
    for i = 1:length(locations_tab)
        filename = append("test_y=", locations_tab(i), "mm_", color_depth, "_", resolution, ".mat");
        plot_position(filename);
    end
end

function plot_position(filename)
    load(filename);
    close(fig_handle);
    plot(mean(cog_horizontal_array), mean(cog_vertical_array), '-b.','MarkerSize',12);
    hold on;

    fileID = fopen('stats.txt','a');
    fprintf(fileID, "%s mean_h = %.4f, mean_v = %.4f, stddev_h = %.4f, stddev_v = %.4f\n",filename, mean(cog_horizontal_array), mean(cog_vertical_array), std(cog_horizontal_array), std(cog_vertical_array));
    fclose(fileID);
end

