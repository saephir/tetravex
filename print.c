#include "print.h"

static int colors = 1;

/* Wypisz kolorowy numerek */
void print_color_number(WINDOW* wnd, int number) {

	int attr = COLOR_PAIR(number+1);
	if(number > 2)
		attr |= A_BOLD;

	attron(attr);
	wprintw(wnd, "%d", number);
	attroff(attr);

}

/*
Rysuj kratkę w wyznaczonym miejscu na oknie
*/
void print_tile(WINDOW* wnd, tile* t, int x, int y) {

	int attr;

	if(t->top != -1 && t->bottom != -1 && t->left != -1 && t->right != -1) {
		if(colors) {

			wmove(wnd, y, x);	
			wprintw(wnd, "##");
			print_color_number(wnd, t->top);
			wprintw(wnd, "##");

			wmove(wnd, y+1, x);
			print_color_number(wnd, t->left);
			wprintw(wnd, "###");
			print_color_number(wnd, t->right);

			wmove(wnd, y+2, x);
			wprintw(wnd, "##");
			print_color_number(wnd, t->bottom);
			wprintw(wnd, "##");

		}
		else {

			wmove(wnd, y, x);
			wprintw(wnd, "##%d##", t->top);
			wmove(wnd, y+1, x);
			wprintw(wnd, "%d###%d", t->left, t->right);
			wmove(wnd, y+2, x);
			wprintw(wnd, "##%d##", t->bottom);

		}
	}

}

/*
Ustaw tryb kolorowy/monochromatyczny
*/
void enable_tile_colors() {

	colors = 1;

}

void disable_tile_colors() {

	colors = 0;

}
