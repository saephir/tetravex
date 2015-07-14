#ifndef PRINT_H
#define PRINT_H

#include <curses.h>
#include "tile.h"

/* Wypisz kolorowy numerek */
void print_color_number(WINDOW* wnd, int number);

/*
Rysuj kratkę w wyznaczonym miejscu na oknie
*/
void print_tile(WINDOW* wnd, tile* t, int x, int y);

/*
Ustaw tryb kolorowy/monochromatyczny
*/
void enable_tile_colors();
void disable_tile_colors();

#endif
