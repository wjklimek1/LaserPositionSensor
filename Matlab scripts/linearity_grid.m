clear;
close all;

load("grid_vertical.mat");

figure();
plot(data_vertical, data_horizontal);
grid on;
grid minor;
xlim([70, 190]);
ylim([100, 200]);

load("grid_vertical2.mat");

figure();
plot(cog_vertical_array, cog_horizontal_array);
grid on;
grid minor;
xlim([70, 190]);
ylim([100, 200]);