#ifndef TILE_H
#define TILE_H

/*
Pustą płytkę oznaczamy -1.
*/
typedef struct tile {
	short top;
	short bottom;
	short left;
	short right;
} tile;

/*
Zwróć pustą płytkę
*/
tile empty_tile();

/*
Zamień wartości płytek
*/
void swap_tiles(tile* t1, tile* t2);

/*
Przesuń płytkę
*/
void move_tile(tile* dest, tile* src);

/*
Sprawdź czy płytka jest pusta
*/
int is_empty(tile* t);

#endif
