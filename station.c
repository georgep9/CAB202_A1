#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "station.h"
#include "collision.h"


#define WIDTH 10
#define HEIGHT 3

static int station_x;
static int station_y;

const char* station_bitmap =
"?########?"
"#  BASE  #"
"?########?";

void draw_station(){
	int top = station_y - HEIGHT/2;
	int left = station_x - WIDTH/2;
	for (int j = 0; j<HEIGHT; j++){
		for (int i = 0; i < WIDTH; i++){
			char pixel = station_bitmap[i+j*WIDTH];
			if (pixel != '?'){
				draw_char(left + i, top + j, pixel);
			}
		}
	}
}

void setup_station(){
	station_x = screen_width()/2;
	station_y = 6;
}

char* get_station_bitmap(){
	return (char*)station_bitmap;
}

void get_station_coords(int* x, int*y){
	*x = station_x;
	*y = station_y;
}

void get_station_size(int* w, int* h){
	*w = WIDTH;
	*h = HEIGHT;
}
