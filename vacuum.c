#include <stdlib.h>
#include <cab202_graphics.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "helpers.h"
#include "vacuum.h"
#include "status.h"
#include "collision.h"
#include "station.h"

#define PI 3.14159265

#define SIZE 13
static double vac_x, vac_y;
static int direction;
static bool in_motion;
static double return_to_base;

static int weight;
static int battery;

const char* vacuum_bitmap =
"?????@@@?????"
"???@@   @@???"
"??@       @??"
"?@         @?"
"?@  _   _  @?"
"@  (o) (o)  @"
"@           @"
"@     +     @"
"?@  .___.  @?"
"?@         @?"
"??@       @??"
"???@@   @@???"
"?????@@@?????"
; // where '?' represents transparent space

void setup_vacuum(){
    int w, h;
    get_screen_size(&w, &h);
    vac_x = w/2;
    vac_y = h/2;
    direction = 90;
    in_motion = false;
    return_to_base = false;
    weight = 0;
    battery = 100;
}

void draw_vacuum(){
    // get top left pixel positions
    int left = vac_x - SIZE/2;
    int top = vac_y - SIZE/2;
    // loops through vacuum bitmap 
    for (int j = 0; j < SIZE; j++){
        for (int i = 0; i < SIZE; i++){
            // get current pixel of bitmap
            char pixel = vacuum_bitmap[i+j*SIZE];
            // draw pixel if not '?' (not transparent)
            if (pixel != '?'){
                draw_char(left+i,top+j,pixel);
            }
        }
    }
}

void rotate_vacuum(){
    srand(time(NULL));
    // add angle between -30 & 30
    direction += rand() % 61 - 30;
    // keep the direction within 0 - 359 range:
    if (direction >= 360){
        direction -= 360;
    } 
    else if (direction < 0){
        direction += 360;
    }
}

bool collides_with(char object[]){

    int x1, y1, w1, h1;
    char* object_bitmap;

    if (strcmp(object,"station")==0){
        get_station_size(&w1, &h1);
        get_station_coords(&x1 - w1, &y1 - h1);
        object_bitmap = get_station_bitmap();
    }

    return pixel_collision(vac_x, vac_y, SIZE, SIZE, (char*)vacuum_bitmap, 
                            x1, y1, w1, h1, object_bitmap);
}

void move_vacuum(){

    int w,h;
    get_screen_size(&w,&h);

    if (in_motion){
        double rad = direction * PI/180;
        // get new x and y
        double new_x = vac_x + 0.2*cos(rad);
        double new_y = vac_y + 0.2*sin(rad);

        // check if collides with station
        if (collides_with("station") == false){
            // move x if vacuum doesn't collide with vertical borders
            if (within_borders(new_x, vac_y, SIZE, SIZE)){
                vac_x = new_x;
            } else { rotate_vacuum(); } // otherwise rotate
            // move y if vacuum doesn't collide with horizontal borders
            if (within_borders(vac_x, new_y, SIZE, SIZE)){
                vac_y = new_y;
            } else { rotate_vacuum(); } // otherwise rotate
        } else { rotate_vacuum(); } // otherwise rotate
    }
}

void update_battery(){

    int seconds_passed = get_seconds_passed();
    if (battery != 0 && (battery-seconds_passed) >= 0){
            battery -= seconds_passed;
        }
}

void update_vacuum(){
    move_vacuum();
    update_battery();
}

void vacuum_collides(){

}

void control_vacuum(char key){
    switch(key){
        case 'i':
            if (within_borders(vac_x, vac_y-1, SIZE, SIZE)) { vac_y--; }
            break;
        case 'j':
            if (within_borders(vac_x-1, vac_y, SIZE, SIZE)) { vac_x--; }
            break;
        case 'k':
            if (within_borders(vac_x, vac_y+1, SIZE, SIZE)) { vac_y++; }
            break;
        case 'l':
            if (within_borders(vac_x+1, vac_y, SIZE, SIZE)) { vac_x++; }
            break;
        case 'p':
            if (in_motion){ in_motion = false; }
            else{ in_motion = true; }
        case 'b':
            return_to_base = true;
    }
}

void set_direction(){
    int new_direction = get_int("Set vacuum direction:");
    if (new_direction < 360 && new_direction >= 0){
        direction = new_direction;
    }
}
int get_direction(){
    return direction;
}


void set_battery_level(){
    int new_level = get_int("New battery level:");
    if (new_level >= 0 && new_level <= 100){
        battery = new_level;
    }
}
int get_battery_level(){
    return battery;
}

void set_vacuum_x(){
    int x = get_int("Set vacuum x:");
    int left_bound = 1;
    int right_bound = screen_width() - 1;
    if (x-SIZE/2 > left_bound && x+SIZE/2 < right_bound){
        vac_x = x;
    }
}

void set_vacuum_y(){
    int y = get_int("Set vacuum y:");
    int top_bound = 5;
    int bottom_bound =  screen_height() - 3;
    if (y-SIZE/2 > top_bound && y+SIZE/2 < bottom_bound){
        vac_y = y;
    }
}

void get_vacuum_coords(double* x, double*y){
    *x = vac_x;
    *y = vac_y;
}

char* get_vacuum_bitmap(){
    return (char*)vacuum_bitmap;
}