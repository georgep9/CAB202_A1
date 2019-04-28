#pragma once


void draw_rubbish();
void setup_rubbish();

int get_dust_amount();
int get_dust_x(int i);
int get_dust_y(int i);
void get_dust_size(int *w, int *h);
char* get_dust_bitmap();


int get_slime_amount();
int get_slime_x(int i);
int get_slime_y(int i);
void get_slime_size(int *w, int *h);
char* get_slime_bitmap();


int get_trash_amount();
int get_trash_x(int i);
int get_trash_y(int i);
void get_trash_size(int *w, int *h);
char* get_trash_bitmap();
