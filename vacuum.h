#pragma once

/*#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include "vacuum.h"*/

void setup_vacuum();

void draw_vacuum();

void move_vacuum();

void update_battery();

void update_vacuum();

void control_vacuum(char key);

void set_direction();
int get_direction();

void set_battery_level();
int get_battery_level();

void set_vacuum_x();
void set_vacuum_y();

void get_vacuum_coords(double* x, double* y);

char* get_vacuum_bitmap();

