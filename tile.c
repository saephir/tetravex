#include "tile.h"

/*
Zwróć pustą płytkę
*/
tile empty_tile() {

	tile t;
	
	t.top = -1;
	t.bottom = -1;
	t.left = -1;
	t.right = -1;

	return t;

}

/*
Zamień wartości płytek
*/
void swap_tiles(tile* t1, tile* t2) {
	
	tile temp = *t1;
	*t1 = *t2;
	*t2 = temp;

}

/*
Przesuń płytkę
*/
void move_tile(tile* dest, tile* src) {

	tile temp;
	temp.top = -1;
	temp.bottom = -1;
	temp.left = -1;
	temp.right = -1;
	
	*dest = *src;
	*src = temp;

}

/*
Sprawdź czy płytka jest pusta
*/
int is_empty(tile* t) {
	return t->top == -1;
}
