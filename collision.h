#pragma once

void setup_collision();

bool pixel_collision(int x0, int y0, int w0, int h0, char* pixels0, int x1, int y1, int w1, int h1, char* pixels1);

bool within_borders(double x, double y, int width, int height);

bool collides_with_station(int x0, int y0, int w0, int h0, char* pixels0);
bool collides_with_vacuum(int x0, int y0, int w0, int h0, char* pixels0);

bool collides_with_dust(int x0, int y0, int w0, int h0, char* pixels0);
bool collides_with_slime(int x0, int y0, int w0, int h0, char* pixels0);
bool collides_with_trash(int x0, int y0, int w0, int h0, char* pixels0);

bool collides_with_anything(int x0, int y0, int w0, int h0, char* pixels0);