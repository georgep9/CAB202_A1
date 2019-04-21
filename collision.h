#pragma once

void setup_collision();

bool pixel_collision(int x0, int y0, int w0, int h0, char* pixels0, int x1, int y1, int w1, int h1, char* pixels1);

bool within_borders(double x, double y, int width, int height);

