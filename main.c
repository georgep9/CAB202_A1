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

// Global variables
bool running;
bool restart;
int main_delay;

void handle_input(char input){

	// time difference between current and reference time,
	// used to update reference time if input dialogs occur
	double time_difference = get_current_time() - get_reference_time();

	switch(input){
		case 'q':
			running = false;
			break;
		case 'r':
			restart = true;
			break;
		case 'y':
			set_battery_level();
			break;
        case 'i':
            control_vacuum('i');
            break;
        case 'j':
            control_vacuum('j');
            break;
        case 'k':
            control_vacuum('k');
            break;
        case 'l':
            control_vacuum('l');
            break;
        case 'p':
        	control_vacuum('p');
        	break;
        case 'm':;
        	int new_delay = get_int("New delay:");
		    if (new_delay > 0){
		        main_delay = new_delay;
		    }
		    break;
	}

	// update reference time:
	// eg. if there is a pause by an input dialog, the reference time
	// should update so it remains the same distance behind the current time.
	// refer to notes in 'status.c'
	double new_reference_time = get_current_time() - time_difference;
	set_reference_time(new_reference_time);

}

void draw_screen(){
	draw_borders();
	draw_status();
	draw_station();
	draw_vacuum();
}

void setup () {
	running = true;
	restart = false;
	main_delay = 1;

	setup_status();
	setup_collision();
	setup_station();
	setup_vacuum();
	
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