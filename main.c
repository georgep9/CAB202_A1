#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "status.h"
#include "vacuum.h"
#include "station.h"
#include "collision.h"
#include "rubbish.h"

// Global variables
bool running;
bool restart;
int main_delay;

void handle_input(char input){

	// time difference between current and reference time (in status),
	// used to update reference time if input dialogs occur
	double time_difference = get_current_time() - get_reference_time();
	// same as above, for battery charging reference time
	double charging_time_diff = get_current_time() * 1000 - get_charging_ref_time();

	switch(input){
		case 'q':
			running = false;
			break;
		case 'r':
			restart = true;
			break;

        case 'm':;
        	int new_delay = get_int("New delay:");
		    if (new_delay > 0){
		        main_delay = new_delay;
		    }
		    break;

		case 'p':
			pause_vacuum();
			break;

        case 'y':
            set_battery_level();
            break;

        case 'v':
            set_vacuum_x();
            set_vacuum_y();
            set_direction();
            break;

        case 'b':
            return_to_base();
            break;

		default:
			control_vacuum(input);
	}

	// update reference time:
	// eg. if there is a pause by an input dialog, the reference time
	// should update so it remains the same distance behind the current time.
	// refer to notes in 'status.c'
	double new_reference_time = get_current_time() - time_difference;
	set_reference_time(new_reference_time);

	// same as above, for battery charging reference time
	double new_charging_ref_time = get_current_time() * 1000 - charging_time_diff;
	set_charging_ref_time(new_charging_ref_time);

}

void draw_screen(){
	draw_borders();
	draw_status();
	draw_station();
	draw_rubbish();
	draw_vacuum();
}

void setup () {
	running = true;
	restart = false;
	main_delay = 1;

	setup_status();
	setup_station();
	setup_vacuum();
	setup_rubbish();
	
	clear_screen();
	draw_screen();
	show_screen();
}

void loop(){
	update_vacuum();
	update_status();

	clear_screen();
	draw_screen();
	show_screen();

	int input = get_char();
	handle_input(input);

	if (restart){ setup(); }
}

int main() {
    setup_screen();

    setup();

    while (running) {
    	timer_pause(main_delay);
    	loop();
    }

    return 0;
}