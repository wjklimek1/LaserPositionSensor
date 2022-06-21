clear;
close all;

diff_horizontal_rgb888_320x240 = [];
diff_vertical_rgb888_320x240 = [];
diff_horizontal_rgb565_320x240 = [];
diff_vertical_rgb565_320x240 = [];
diff_horizontal_rgb888_160x120 = [];
diff_vertical_rgb888_160x120 = [];
diff_horizontal_rgb565_160x120 = [];
diff_vertical_rgb565_160x120 = [];

px_horizontal_rgb888_320x240 = [139.0535, 149.5994, 159.8773, 169.8177, 177.9399];
px_vertical_rgb888_320x240 = [102.5536, 111.5819, 119.2616, 126.6651, 137.1395];
px_horizontal_rgb565_320x240 = [133.1442, 147.1482, 160.9627, 173.4439, 183.0120];
px_vertical_rgb565_320x240 = [96.6970, 107.5740, 120.8572, 134.1572, 144.0352];
px_horizontal_rgb888_160x120 = [68.9339, 74.2299, 79.4146, 84.4694, 88.4526];
px_vertical_rgb888_160x120 = [51.6462, 56.1677, 59.9268, 63.7212, 68.6645];
px_horizontal_rgb565_160x120 = [66.8599, 73.6799, 80.5464, 86.7517, 91.6152];
px_vertical_rgb565_160x120 = [48.6255, 53.9477, 60.3179, 67.0715, 72.1248];

diff_horizontal_rgb888_320x240 = difference(px_horizontal_rgb888_320x240);
diff_vertical_rgb888_320x240 = difference(px_vertical_rgb888_320x240);
diff_horizontal_rgb888_160x120 = difference(px_horizontal_rgb888_160x120);
diff_vertical_rgb888_160x120 = difference(px_vertical_rgb888_160x120);
diff_horizontal_rgb565_320x240 = difference(px_horizontal_rgb565_320x240);
diff_vertical_rgb565_320x240 = difference(px_vertical_rgb565_320x240);
diff_horizontal_rgb565_160x120 = difference(px_horizontal_rgb565_160x120);
diff_vertical_rgb565_160x120 = difference(px_vertical_rgb565_160x120);

fprintf("RGB888 320x240 horizontal, avg = %.4f, stddev = %.4f\n", mean(diff_horizontal_rgb888_320x240)*2, std(diff_horizontal_rgb888_320x240)*2);
fprintf("RGB888 320x240 vertical  , avg = %.4f, stddev = %.4f\n", mean(diff_vertical_rgb888_320x240)*2, std(diff_vertical_rgb888_320x240)*2);
fprintf("RGB888 160x120 horizontal, avg = %.4f, stddev = %.4f\n", mean(diff_horizontal_rgb888_160x120)*2, std(diff_horizontal_rgb888_160x120)*2);
fprintf("RGB888 160x120 vertical  , avg = %.4f, stddev = %.4f\n", mean(diff_vertical_rgb888_160x120)*2, std(diff_vertical_rgb888_160x120)*2);
fprintf("RGB565 320x240 horizontal, avg = %.4f, stddev = %.4f\n", mean(diff_horizontal_rgb565_320x240)*2, std(diff_horizontal_rgb565_320x240)*2);
fprintf("RGB565 320x240 vertical  , avg = %.4f, stddev = %.4f\n", mean(diff_vertical_rgb565_320x240)*2, std(diff_vertical_rgb565_320x240)*2);
fprintf("RGB565 160x120 horizontal, avg = %.4f, stddev = %.4f\n", mean(diff_horizontal_rgb565_160x120)*2, std(diff_horizontal_rgb565_160x120)*2);
fprintf("RGB565 160x120 vertical  , avg = %.4f, stddev = %.4f\n", mean(diff_vertical_rgb565_160x120)*2, std(diff_vertical_rgb565_160x120)*2);

fprintf("\n");
fprintf("RGB888 320x240 horizontal, a = %.4f\n", 0.5/mean(diff_horizontal_rgb888_320x240));
fprintf("RGB888 320x240 vertical,   a = %.4f\n", 0.5/mean(diff_vertical_rgb888_320x240));
fprintf("RGB888 160x120 horizontal, a = %.4f\n", 0.5/mean(diff_horizontal_rgb888_160x120));
fprintf("RGB888 160x120 vertical,   a = %.4f\n", 0.5/mean(diff_vertical_rgb888_160x120));
fprintf("RGB565 320x240 horizontal, a = %.4f\n", 0.5/mean(diff_horizontal_rgb565_320x240));
fprintf("RGB565 320x240 vertical  , a = %.4f\n", 0.5/mean(diff_vertical_rgb565_320x240));
fprintf("RGB565 160x120 horizontal, a = %.4f\n", 0.5/mean(diff_horizontal_rgb565_160x120));
fprintf("RGB565 160x120 vertical  , a = %.4f\n", 0.5/mean(diff_vertical_rgb565_160x120));

function diff_array = difference(input)
    diff_array = [];
    for i = 1:length(input) - 1
        diff = input(length(input) - i + 1) - input(length(input) - i);
        diff_array = [diff_array, diff];
    end
end

