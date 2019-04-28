#include <cab202_graphics.h>
#include <stdlib.h>
#include <string.h>
#include "collision.h"
#include "vacuum.h"
#include "station.h"
#include "rubbish.h"
#include "status.h"

bool pixel_collision(int x0, int y0, int w0, int h0, char* pixels0, int x1, int y1, int w1, int h1, char* pixels1){
    for (int j = 0; j < h0; j++){
        for (int i = 0; i < w0; i++){
            if (pixels0[i + j*w0] != '?'){
                int x = x0 + i;
                int y = y0 + j;
                if (
                    x >= x1 &&
                    x < x1 + w1 &&
                    y >= y1 &&
                    y < y1 + h1 &&
                    pixels1[(x-x1)+(y-y1)*w1] != '?'
                    ) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool within_borders(double x, double y, int width, int height){
	int x0 = x - width/2;
	int y0 = y - width/2;
	int x1 = x + width/2;
	int y1 = y + width/2;

    int top_border = 5;
    int bottom_border = screen_height() - 3;
    int left_border = 1;
    int right_border = screen_width() - 1;
	
	if (x0 >= left_border && y0 >= top_border && x1 < right_border && y1 < bottom_border){
		return true;
	}
	return false;
}

bool collides_with_station(int x0, int y0, int w0, int h0, char* pixels0){

    x0 -= w0/2;
    y0 -= h0/2;

    int x1, y1, w1, h1;
    char* pixels1;

    get_station_size(&w1, &h1); // get width & height
    get_station_coords(&x1, &y1); // get its coords
    x1 -= w1/2; // left position of object 
    y1 -= h1/2; // right positon of object
    pixels1 = get_station_bitmap(); // get object bitmap

    // return bool result of whether collision occurs
    return pixel_collision(x0, y0, w0, h0, pixels0, x1, y1, w1, h1, pixels1);
}

bool collides_with_vacuum(int x0, int y0, int w0, int h0, char* pixels0){

    x0 -= w0/2;
    y0 -= h0/2;

    double x1, y1;
    int w1, h1;
    char* pixels1;

    get_vacuum_size(&w1, &h1); // get width & height
    get_vacuum_coords(&x1, &y1); // get its coords
    x1 -= w1/2; // left position of object 
    y1 -= h1/2; // right positon of object
    pixels1 = get_vacuum_bitmap(); // get object bitmap

    // return bool result of whether collision occurs
    return pixel_collision(x0, y0, w0, h0, pixels0, x1, y1, w1, h1, pixels1);
}

bool collides_with_dust(int x0, int y0, int w0, int h0, char* pixels0){
    x0 -= w0/2;
    y0 -= h0/2;

    int amount = get_dust_amount();
    int x1, y1;
    int w1, h1;
    get_dust_size(&w1, &h1);
    char*pixels1 = get_dust_bitmap();

    for (int i = 0; i < amount; i++){
        x1 = get_dust_x(i) - w1/2;
        y1 = get_dust_y(i) - h1/2;
        if(pixel_collision(x0,y0,w0,h0,pixels0,x1,y1,w1,h1,pixels1))
        { return true; }
    }
    return false;
}


bool collides_with_slime(int x0, int y0, int w0, int h0, char* pixels0){
    x0 -= w0/2;
    y0 -= h0/2;

    int amount = get_slime_amount();
    int x1, y1;
    int w1, h1;
    get_slime_size(&w1, &h1);
    char*pixels1 = get_slime_bitmap();

    for (int i = 0; i < amount; i++){
        x1 = get_slime_x(i) - w1/2;
        y1 = get_slime_y(i) - h1/2;
        if(pixel_collision(x0,y0,w0,h0,pixels0,x1,y1,w1,h1,pixels1))
        { return true; }
    }
    return false;
}


bool collides_with_trash(int x0, int y0, int w0, int h0, char* pixels0){
    x0 -= w0/2;
    y0 -= h0/2;

    int amount = get_trash_amount();
    int x1, y1;
    int w1, h1;
    get_trash_size(&w1, &h1);
    char*pixels1 = get_trash_bitmap();

    for (int i = 0; i < amount; i++){
        x1 = get_trash_x(i) - w1/2;
        y1 = get_trash_y(i) - h1/2;
        if(pixel_collision(x0,y0,w0,h0,pixels0,x1,y1,w1,h1,pixels1))
        { return true; }
    }
    return false;
}

bool collides_with_anything(int x0, int y0, int w0, int h0, char* pixels0){
    if (!within_borders(x0, y0, w0, h0)
        || collides_with_station(x0,y0,w0,h0,pixels0)
        || collides_with_vacuum(x0,y0,w0,h0,pixels0)
        || collides_with_dust(x0,y0,w0,h0,pixels0)
        || collides_with_slime(x0,y0,w0,h0,pixels0)
        || collides_with_trash(x0,y0,w0,h0,pixels0)) { return true; }
    return false;
}

