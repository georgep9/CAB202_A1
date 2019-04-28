#include <cab202_graphics.h>
#include <stdlib.h>
#include <time.h>
#include "helpers.h"
#include "collision.h"
#include "station.h"
#include "vacuum.h"
#include "rubbish.h"
#include "status.h"

#define DUST_SIZE 1
#define MAX_DUST 1000
static int dust_x[MAX_DUST];
static int dust_y[MAX_DUST];
static int dust_amount;
const char* dust_bitmap = ".";

#define SLIME_SIZE 5
#define SLIME_MAX 10
static int slime_x[SLIME_MAX];
static int slime_y[SLIME_MAX];
static int slime_amount;
const char* slime_bitmap = 
"??~??"
"?~~~?"
"~~~~~"
"?~~~?"
"??~??"; // where '?' represents transparency

#define TRASH_WIDTH 11
#define TRASH_HEIGHT 6
#define TRASH_MAX 5
static int trash_x[TRASH_MAX];
static int trash_y[TRASH_MAX];
static int trash_amount;
const char* trash_bitmap =
"?????&?????"
"????&&&????"
"???&&&&&???"
"??&&&&&&&??"
"?&&&&&&&&&?"
"&&&&&&&&&&&"; // where '?' represents transparency

void draw_dust(int index){
	int x = dust_x[index];
	int y = dust_y[index];
	draw_char(x,y,dust_bitmap[0]);
}

void draw_slime(int index){
	int left = slime_x[index] - SLIME_SIZE/2;
	int top = slime_y[index] - SLIME_SIZE/2;
	for (int j = 0; j < SLIME_SIZE; j++){
		for (int i = 0; i < SLIME_SIZE; i++){
			char pixel = slime_bitmap[i+j*SLIME_SIZE];
			if (pixel != '?') {
				draw_char(left+i,top+j,pixel);
			}
		}
	}
}

void draw_trash(int index){
	int left = trash_x[index] - TRASH_WIDTH/2;
	int top = trash_y[index] - TRASH_HEIGHT/2;
	for (int j = 0; j < TRASH_HEIGHT; j++){
		for (int i = 0; i < TRASH_WIDTH; i++){
			char pixel = trash_bitmap[i+j*TRASH_WIDTH];
			if (pixel != '?') {
				draw_char(left+i,top+j,pixel);
			}
		}
	}
}

void draw_rubbish(){

	int i;

	

	for (i = 0; i < slime_amount; i++) { draw_slime(i); }

	for (i = 0; i < trash_amount; i++) { draw_trash(i); }

	for (i = 0; i < dust_amount; i++){ draw_dust(i); }
}

int random_x(){ return 1 + rand() % (screen_width() - 2); }
int random_y(){ return 5 + rand() % (screen_height() - 8); }

void generate_dust(){

	// 
	for (int i = 0; i < dust_amount; i++){
		int x = random_x();
		int y = random_y();

		while ( collides_with_anything(x,y,DUST_SIZE,DUST_SIZE,(char*)dust_bitmap)) { 
			x = random_x();
			y = random_y();
		}
		dust_x[i] = x;
		dust_y[i] = y;
	}
}

void generate_slime(){

	// 
	for (int i = 0; i < slime_amount; i++){
		int x = random_x();
		int y = random_y();

		while ( collides_with_anything(x,y,SLIME_SIZE,SLIME_SIZE,(char*)slime_bitmap)) { 
			x = random_x();
			y = random_y();
		}
		slime_x[i] = x;
		slime_y[i] = y;
	}
}

void generate_trash(){

	// 
	for (int i = 0; i < trash_amount; i++){
		int x = random_x();
		int y = random_y();

		while ( collides_with_anything(x,y,TRASH_WIDTH,TRASH_HEIGHT,(char*)trash_bitmap)) { 
			x = random_x();
			y = random_y();
		}
		trash_x[i] = x;
		trash_y[i] = y;
	}
}

void setup_rubbish(){
	dust_amount = get_int("How many pieces of dust?");
	slime_amount = get_int("How many pieces of slime?");
	trash_amount = get_int("How many pieces of trash?");
	generate_dust();
	generate_slime();
	generate_trash();
}

int get_dust_x(int i){ return dust_x[i]; }
int get_dust_y(int i){ return dust_y[i]; }
int get_dust_amount() { return dust_amount; }
char* get_dust_bitmap() { return (char*)dust_bitmap; }
void get_dust_size(int *w, int *h) { 
	*w = DUST_SIZE;
	*h = DUST_SIZE;
}

int get_slime_x(int i) { return slime_x[i]; }
int get_slime_y(int i) { return slime_y[i]; }
int get_slime_amount() { return slime_amount; }
char* get_slime_bitmap() { return (char*)slime_bitmap; }
void get_slime_size(int *w, int *h) {
	*w = SLIME_SIZE;
	*h = SLIME_SIZE;
}

int get_trash_x(int i) { return trash_x[i]; }
int get_trash_y(int i) { return trash_y[i]; }
int get_trash_amount() { return trash_amount; }
char* get_trash_bitmap() { return (char*)trash_bitmap; }
void get_trash_size(int *w, int *h) {
	*w = TRASH_WIDTH;
	*h = TRASH_HEIGHT;
}
