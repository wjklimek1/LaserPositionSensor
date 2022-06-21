load("gaussian_RGB888_320x240.mat");
%load("gaussian_RGB888_160x120.mat");
%load("gaussian_RGB565_320x240.mat");
%load("gaussian_RGB565_160x120.mat");

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