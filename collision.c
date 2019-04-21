#include <cab202_graphics.h>
#include "collision.h"

static int top_border;
static int bottom_border;
static int left_border;
static int right_border;

void setup_collision(){
	top_border = 5;
	bottom_border = screen_height() - 3;
	left_border = 1;
	right_border = screen_width() - 1;
}

bool pixel_collision(int x0, int y0, int w0, int h0, char* pixels0, int x1, int y1, int w1, int h1, char* pixels1){
    int x;
    int y;
    for (int j = 0; j < h0; j++){
        for (int i = 0; i < w0; i++){
            if (pixels0[i + j*w0] != '?'){
                x = x0 + i;
                y = y0 + j;
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
	
	if (x0 >= left_border && y0 >= top_border && x1 < right_border && y1 < bottom_border){
		return true;
	}
	return false;
}