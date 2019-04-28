#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "vacuum.h"

static double elasped_time; // elasped time of simulation
static double reference_time; // reference time to get seconds passed
static double seconds_passed; // time between current and reference

double check_double;

void draw_borders(){

	int width, height;
	get_screen_size(&width, &height);

	// draw horizontal border lines '-'
	draw_line(0,0,width-1,0,'-');
	draw_line(0,2,width-1,2,'-');
	draw_line(0,4,width-1,4,'-');
	draw_line(0,height-3,width-1,height-3,'-');

	// draw vertical border lines '|'
	draw_line(0,0,0,height-3,'|');
	draw_line((width-1)/3, 0, (width-1)/3, 4, '|');
	draw_line(((width-1)*2/3), 0, ((width-1)*2/3), 4, '|');
	draw_line(width-1, 0, width-1, height-3, '|');

	// draw intersections '+'
	for (int row = 0; row <= 2; row++){
		for (int col = 0; col <= 3; col++){
			draw_char(col*(width-1)/3, row*2, '+');
		}
	}
	draw_char(0, height-3, '+');
	draw_char(width-1, height-3, '+');

}

void setup_status(){
	/*direction = 0;
	battery_level = 100;
	weight = 0;
	litter = 0;*/
	elasped_time = 0;
	reference_time = get_current_time();
	seconds_passed = 0;

	check_double = 0;
}

// draw the status 
void draw_status(){

	int width = screen_width();

	int direction = get_direction();
	draw_formatted(((width-1-7)*3/6),1,"Direction: %d",direction);

	int total_seconds = (int) elasped_time;
	int minutes = total_seconds / 60;
	int seconds = total_seconds % 60;
	draw_formatted(((width-1)/6)-7,3,"Elapsed: %02d:%02d", minutes, seconds);

	int battery = get_battery_level();
	draw_formatted(((width-1)*5/6)-6,1,"Battery: %d%", battery);

	draw_formatted(1,1,"Diagnosis number: %f", check_double);

}

void update_status(){

	double current_time = get_current_time();

	// check if 1 second has passed
	seconds_passed = current_time - reference_time;
	if ((int)seconds_passed == 1){
		elasped_time++;
		reference_time = current_time;
	} 
	// in the case of a large loop delay
	else if (seconds_passed > 1){
		elasped_time += (int)seconds_passed;
		reference_time = current_time;
	}
}

double get_seconds_passed(){
	return seconds_passed;
}

void set_reference_time(double t){
	reference_time = t;
}
double get_reference_time(){
	return reference_time;
}



void diagnose_double(double number){ check_double = number; }
