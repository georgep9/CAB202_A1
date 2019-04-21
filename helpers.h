#pragma once

/**
 *	helpers.h, by Lawrence Buckingham.
 *  Student Id: n1839870
 *
 *	(C) 2017-2019 Queensland University of Technology
 *
 *	A small collection of helper functions.
 *
 *	To compile this function into your program:
 *      ZDK_PATH=(the name of the directory that contains libzdk.a)
 *		gcc your_file_1.c your_file_2.c ... helpers.c -o your_program -std-gnu99 -Wall -Werror -I${ZDK_PATH} -L${ZDK_PATH} -lzdk -lncurses -lm
 *
 *	Programs that use the functions should place helpers.h somewhere in the include
 *	path and #include it.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cab202_graphics.h>

void draw_rect( int left, int top, int right, int bottom, int ch );
void draw_formatted_lines( int x0, int y0, const char * format, ... );
void erase_row( int y );
int get_chars( char * prompt, char * buffer, const int max_len );
int get_int( char * prompt );
