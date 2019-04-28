#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "helpers.h"
#include "vacuum.h"
#include "status.h"
#include "collision.h"
#include "station.h"

#define PI 3.14159265 // used for radians

#define SIZE 13 // width and height of vacuum
static double x_centre, y_centre; // position of vacuum center
static int direction; // direction of vacuum
static int weight; // weight vacuum holds
static int battery; // battery level of vacuum

static bool in_motion; // flag for when vacuum is paused 'p'
static bool return_mode; // flag for when vacuum should return to base
static bool vacuum_docked; // flag for when vacuum is docked at charging station

static double charging_ref_time; // reference time to check if 30ms has passed

// pixel/char bitmap of vacuum
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

// assign default values to vacuum
void setup_vacuum(){
    int w, h;
    get_screen_size(&w, &h);
    x_centre = w/2; // initial x to be middle of horizontal axis
    y_centre = h/2; // initial y to be middle of vertical axis
    direction = 90; // initial direction facing east (right)
    in_motion = false; // vacuum initially paused
    return_mode = false; // initially does not return
    weight = 0; // initially has no weight
    battery = 100; // initially has full battery
    vacuum_docked = false; // initially not docked at charging station
    charging_ref_time = get_current_time() * 1000; // set reference time to current time
}

// draws the vacuum at coords
void draw_vacuum(){
    // get top left pixel positions
    int left = x_centre - SIZE/2;
    int top = y_centre - SIZE/2;
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

// check if the vacuum at a given position, collides with described object
bool collides_with(int x, int y, char object[]){

    x -= SIZE/2; // left position of vacuum at given x
    y -= SIZE/2; // top position of vacuum at given y

    // declare variables for described oject
    int x1, y1, w1, h1;
    char* object_bitmap;

    // if object is "station"
    if (strcmp(object,"station")==0){
        get_station_size(&w1, &h1); // get width & height
        get_station_coords(&x1, &y1); // get its coords
        x1 -= w1/2; // left position of object 
        y1 -= h1/2; // right positon of object
        object_bitmap = get_station_bitmap(); // get object bitmap
    }

    // return bool result of whether pixel collision occurs (collision.c)
    return pixel_collision(x, y, SIZE, SIZE, (char*)vacuum_bitmap, 
                            x1, y1, w1, h1, object_bitmap);
}

// moves the vacuum to charging station by constantly checking position relative to station
void move_vacuum_to_base(){

    //get position of station
    int station_x, station_y;
    get_station_coords(&station_x, &station_y);

    double x_distance = station_x - x_centre; // x distance of station from vacuum
    double y_distance = station_y - y_centre; // y distance of station from vacuum
    double new_direction = atan(y_distance/x_distance); // direction (rad) to station
    if (x_distance < 0){ new_direction += PI; } // adjust direction to be in 360 range
    double new_x = x_centre + 0.2*cos(new_direction); // new x to be
    double new_y = y_centre + 0.2*sin(new_direction); // new y to be

    // if new x & y collides with station, vacuum becomes docked
    if (collides_with(new_x,new_y,"station")){ vacuum_docked = true }
    else { 
        vacuum_docked = false;
        // else, set vacuum x & y to be new x & y if within borders
        if (within_borders(new_x, y_centre, SIZE, SIZE)) { x_centre = new_x; }
        if (within_borders(x_centre, new_y, SIZE, SIZE)) { y_centre = new_y; }
    }
}

// rotates vacuum by changing direciton either left or right by a random angle between 30 and 60 degrees
void rotate_vacuum(){
    srand(time(NULL));
    // add angle between -30 & 30
    direction += rand() % 61 - 30;
    // keep the direction within 0 - 359 range:
    if (direction >= 360){ direction -= 360; } 
    else if (direction < 0){ direction += 360; }
}

// move vacuum by default behaviour
void move_vacuum(){

    // get screen width & height
    int w,h;
    get_screen_size(&w,&h);

    // if not paused and is not in return mode
    if (in_motion && return_mode == false){
        double rad = direction * PI/180; // convert direction to radians
        double new_x = x_centre + 0.2*cos(rad); // new x to be
        double new_y = y_centre + 0.2*sin(rad); // new y to be

        // check if collides with station
        if (collides_with(new_x,new_y,"station") == false){

            // move x if vacuum doesn't collide with vertical borders
            if (within_borders(new_x, y_centre, SIZE, SIZE)){  x_centre = new_x; } 
            else { rotate_vacuum(); } // otherwise rotate

            // move y if vacuum doesn't collide with horizontal borders
            if (within_borders(x_centre, new_y, SIZE, SIZE)){ y_centre = new_y; } 
            else { rotate_vacuum(); } // otherwise rotate

        } else { rotate_vacuum(); } // otherwise rotate
    }

    // if vacuum is in return mode, move to base
    if (return_mode){ move_vacuum_to_base(); }
}

// update battery status
void update_battery(){
    double seconds_passed = get_seconds_passed(); // get seconds passed for battery discharge

    // get miliseconds passed as difference between charging ref. time and current time
    double ms_passed = get_current_time()*1000-charging_ref_time;

    // if vacuum is docked
    if (vacuum_docked){
        // if battery is less than 100
        if (battery < 100){
            // the battery should charges at a rate of 100% in 3 seconds,
            // that means it should incremenet 1% in 30ms. -->
            // if the ms passed is 30, increment battery by 1
            if ((int)ms_passed == 30) { 
                battery += 1;
            }
            else if((int)ms_passed > 30){
                // in the case of a large main loop delay
                battery += (int)ms_passed / 30;
            }
        }
        // if the battery is/reaches 100% while docked
        else if (battery == 100){ 
            vacuum_docked = false; // the vacuum should no longer be docked
            return_mode = false; // the vacuum should no longer need to return to station
        }
    }
    // if the battery is not docked, 
    // and the battery isn't 0% && the next discharge is does not give a negative
    else if (battery != 0 && (battery-(int)seconds_passed) >= 0){
        // discharge the battery by seconds passed (1% if appropriate main loop delay given)
        battery -= (int)seconds_passed;
    }

    // update charging reference time when ms passed reaches 30ms  
    if ((int)ms_passed >= 30){
        charging_ref_time = get_current_time() * 1000; // set to current time
    }
}

// update the vacuum to move and change battery status
void update_vacuum(){
    // set vacuum to return mode when battery drops below 25%
    if (battery < 25) { return_mode = true; }

    move_vacuum(); // move vacuum, default behaviour
    update_battery(); // update battery status
}

// move the vacuum by keys
void control_vacuum(char key){
    switch(key){
        case 'i': // move up if new position is within borders
            if (within_borders(x_centre, y_centre-1, SIZE, SIZE) 
                && !collides_with(x_centre, y_centre-1, "station")) { 
                y_centre--; // move up
            }
            break;
        case 'j': // move left if new position is within borders
            if (within_borders(x_centre-1, y_centre, SIZE, SIZE)
                && !collides_with(x_centre-1, y_centre, "station")) { 
                x_centre--; // move left
            }
            break;
        case 'k': // move down if new position is within borders
            if (within_borders(x_centre, y_centre+1, SIZE, SIZE)
                && !collides_with(x_centre, y_centre+1, "station")) { 
                y_centre++; // move down
            }
            break;
        case 'l': // move right if new position is within borders
            if (within_borders(x_centre+1, y_centre, SIZE, SIZE)
                && !collides_with(x_centre+1, y_centre, "station")) { 
                x_centre++;  // move right
            }
            break;
    }
}

// function for user to enable return mode
void return_to_base(){ return_mode = true; }

// function for user to pause or unpause vacuum default behaviour
void pause_vacuum(){
    if (in_motion){ in_motion = false; }
    else { in_motion = true; }    
}

// function to set new direction by user
void set_direction(){
    int new_direction = get_int("Set vacuum direction:");
    // set if appropriate direction given
    if (new_direction < 360 && new_direction >= 0){
        direction = new_direction;
    }
}
int get_direction(){ return direction; } // get vacuum direction (for status)

// function for user to set battery level
void set_battery_level(){
    int new_level = get_int("New battery level:");
    // set if appropriate level given
    if (new_level >= 0 && new_level <= 100){
        battery = new_level;
    }
}
int get_battery_level(){ return battery; } // get vacuum battery (for status)

// function for user to set vacuum x
void set_vacuum_x(){
    int x = get_int("Set vacuum x:");
    // set if within borders
    if (within_borders(x, y_centre, SIZE, SIZE)){
        x_centre = x;
    }
}

// function for user to set vacuum y
void set_vacuum_y(){
    int y = get_int("Set vacuum y:");
    // set if within borders
    if (within_borders(x_centre, y, SIZE, SIZE)){
        y_centre = y;
    }
}

// 
void get_vacuum_coords(double* x, double*y){
    *x = x_centre;
    *y = y_centre;
}

char* get_vacuum_bitmap(){ return (char*)vacuum_bitmap; }

double get_charging_ref_time(){ return charging_ref_time; }
void set_charging_ref_time(double t) { charging_ref_time = t; }