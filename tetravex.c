#include "tile.h"
#include "print.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define TILE_WIDTH 5
#define TILE_HEIGHT 3
#define INIT_X 5
#define INIT_Y 5
#define RANDOM_ITERATIONS 1000
#define MIN_SIZE 2
#define MAX_SIZE 5

#define HELP_LINE "'a1>b3' moves tile a1 to b3; 'a1>pool' moves tile a1 to the pool..."
#define HELP_LINE2 "'<<' moves all tiles left, analogic to: '>>', '^^', 'vv'..."

/* Rozmiar pola gry */
static int TILES_COUNT = 3;

/* Pole gry */
static tile game[MAX_SIZE][MAX_SIZE];

/* Pula płytek (pool) */
static tile pool[MAX_SIZE][MAX_SIZE];

/*
Rysuj ramkę
*/
void draw_frame(WINDOW* wnd) {
	wborder(wnd, 0, 0, 0, 0, 0, 0, 0, 0);
	wmove(wnd, 0, 3);
	wprintw(wnd, "%s", "Tetravex");
}

/*
Wypisz zawartość pola gry
*/
void print_game(WINDOW* wnd) {

	int i,j;

	wmove(wnd, 3, 5);
	
	wprintw(wnd, "%s", "Game field:");

	/* wypisz współrzędne znakowe */
	for(i=0;i<TILES_COUNT;i++) {
		wmove(wnd, 8+i*3, 2);
		wprintw(wnd, "%c", 'A'+i);
	}

	/* wypisz współrzędne numeryczne */
	for(i=0;i<TILES_COUNT;i++) {
		wmove(wnd, 5, 5+i*5+2);
		wprintw(wnd, "%d.", i+1);
	}

	for(i=0;i<TILES_COUNT;i++) {
		for(j=0;j<TILES_COUNT;j++) {
			
			print_tile(wnd, &game[i][j], 5+TILE_WIDTH*i, 7+TILE_HEIGHT*j);

		}
	}
	
}

/*
Wypisz zawartość stosu
*/
void print_pool(WINDOW* wnd) {

	int i,j;

	/* wypisz współrzędne znakowe */
	for(i=0;i<TILES_COUNT;i++) {
		wmove(wnd, 4+(i*3)+i, 37);
		wprintw(wnd, "%c", 'A'+TILES_COUNT+i);
	}

	/* wypisz współrzędne numeryczne */
	for(i=0;i<TILES_COUNT;i++) {
		wmove(wnd, 1, 40+i*5+2+i);
		wprintw(wnd, "%d.", i+1);
	}

	wmove(wnd, 1, 35);
	
	wprintw(wnd, "Pool:");

	for(i=0;i<TILES_COUNT;i++) {
		for(j=0;j<TILES_COUNT;j++) {

			print_tile(wnd, &pool[i][j], 40+TILE_WIDTH*i+i, 3+TILE_HEIGHT*j+j);

		}
	}

}

/* Spróbuj przesunąć wszystkie płytki w górę */
void push_top() {
	
	int i, j;
	int isok = 1;


	/* Sprawdź czy można przesuwać - wszystkie płytki z y=0 muszą być puste */
	for(i=0;i<TILES_COUNT;i++) {

		if(!is_empty(&game[i][0])) {

			isok = 0;
			break;

		}

	}

	if(!isok)
		return;

	/* Przesuń wszystkie płytki w górę (z góry do dołu) */
	for(i=1;i<TILES_COUNT;i++) {

		for(j=0;j<TILES_COUNT;j++) {

			if(!is_empty(&game[j][i])) {
				game[j][i-1] = game[j][i];
				game[j][i] = empty_tile();
			}

		}

	}

}

/* Spróbuj przesunąć wszystkie płytki w dół */
void push_bottom() {
	
	int i, j;
	int isok = 1;


	/* Sprawdź czy można przesuwać - wszystkie płytki z y=TILES_COUNT-1 muszą być puste */
	for(i=0;i<TILES_COUNT;i++) {

		if(!is_empty(&game[i][TILES_COUNT-1])) {

			isok = 0;
			break;

		}

	}

	if(!isok)
		return;

	/* Przesuń wszystkie płytki w górę (z dołu do góry) */
	for(i=TILES_COUNT-2;i>=0;i--) {

		for(j=0;j<TILES_COUNT;j++) {

			if(!is_empty(&game[j][i])) {
				game[j][i+1] = game[j][i];
				game[j][i] = empty_tile();
			}

		}

	}

}

/* Spróbuj przesunąć wszystkie płytki w lewo */
void push_left() {
	
	int i, j;
	int isok = 1;


	/* Sprawdź czy można przesuwać - wszystkie płytki z x=0 muszą być puste */
	for(i=0;i<TILES_COUNT;i++) {

		if(!is_empty(&game[0][i])) {

			isok = 0;
			break;

		}

	}

	if(!isok)
		return;

	/* Przesuń wszystkie płytki w lewo (z lewej do prawej) */
	for(i=1;i<TILES_COUNT;i++) {

		for(j=0;j<TILES_COUNT;j++) {

			if(!is_empty(&game[i][j])) {
				game[i-1][j] = game[i][j];
				game[i][j] = empty_tile();
			}

		}

	}

}

/* Spróbuj przesunąć wszystkie płytki w prawo */
void push_right() {
	
	int i, j;
	int isok = 1;


	/* Sprawdź czy można przesuwać - wszystkie płytki z x=TILES_COUNT-1 */
	for(i=0;i<TILES_COUNT;i++) {

		if(!is_empty(&game[TILES_COUNT-1][i])) {

			isok = 0;
			break;

		}

	}

	if(!isok)
		return;

	/* Przesuń wszystkie płytki w prawo (z prawej do lewej) */
	for(i=TILES_COUNT-2;i>=0;i--) {

		for(j=0;j<TILES_COUNT;j++) {

			if(!is_empty(&game[i][j])) {
				game[i+1][j] = game[i][j];
				game[i][j] = empty_tile();
			}

		}

	}

}

/*
Inicjalizuj grę
*/
void init() {

	short i, j, index, temp, temp2;
	short len;
	short* random;
	tile temp_tile;

	/* Wyczyść obydwie tablice */
	for(i=0;i<TILES_COUNT;i++) {
		for(j=0;j<TILES_COUNT;j++) {
			game[i][j].top = -1;
			game[i][j].bottom = -1;
			game[i][j].left = -1;
			game[i][j].right = -1;
			pool[i][j].top = -1;
			pool[i][j].bottom = -1;
			pool[i][j].left = -1;
			pool[i][j].right = -1;
		}
	}

	/* Obliczamy liczbę unikalnych wartości dla danej układanki */
	len = 2*TILES_COUNT*(TILES_COUNT-1);
	random = (short*)malloc(sizeof(short)*len);

	/* Generujemy losowe liczby z zakresu 0-9 */
	for(i=0;i<len;i++)
		random[i] = rand() % 10;

	/* Kopiujemy wartości do odpowiednich komórek w puli (pool) */

	index = 0;

	for(i=0;i<TILES_COUNT;i++) {
		for(j=0;j<TILES_COUNT;j++) {

			if(j < TILES_COUNT-1) {
				temp = random[index++];
				pool[j][i].right = temp;
				pool[j+1][i].left = temp;
			}

			if(i < TILES_COUNT-1) {
				temp = random[index++];
				pool[j][i].bottom = temp;
				pool[j][i+1].top = temp;
			}

		}
	}

	free(random);

	/* Uzupełniamy nieistotnymi wartościami */
	for(i=0;i<TILES_COUNT;i++) {
		for(j=0;j<TILES_COUNT;j++) {
			if(pool[i][j].top == -1)
				pool[i][j].top = rand() % 10;
			if(pool[i][j].bottom == -1)
				pool[i][j].bottom = rand() % 10;
			if(pool[i][j].left == -1)
				pool[i][j].left = rand() % 10;
			if(pool[i][j].right == -1)
				pool[i][j].right = rand() % 10;
		}
	}

	/* Dokonujemy losowych modyfikacji */
	for(i=0;i<RANDOM_ITERATIONS;i++) {

		temp = rand() % (TILES_COUNT*TILES_COUNT);
		temp2 = rand() % (TILES_COUNT*TILES_COUNT);

		if(temp != temp2)
			swap_tiles(&pool[temp/TILES_COUNT][temp%TILES_COUNT], &pool[temp2/TILES_COUNT][temp2%TILES_COUNT]);
		else continue;
		
	}

}

/*
Zweryfikuj
0 - gra trwa
1 - wygrana
*/
int verify() {
	
	int i, j;
	
	for(j=0;j<TILES_COUNT;j++) {
		for(i=0;i<TILES_COUNT;i++) {

			if(is_empty(&game[i][j]))
				return 0;

			if(i < TILES_COUNT-1) {

				if(game[i][j].right != game[i+1][j].left)
					return 0;

			}

			if(j < TILES_COUNT-1) {

				if(game[i][j].bottom != game[i][j+1].top)
					return 0;

			}

		}
	}

	return 1;

}

int main(int argn, char** argv) {
	
	int xsize, ysize;
	char c, c2;
	int i, i2;
	int x, y;

	WINDOW* wnd;
	char buf[50];
	tile* temp_tile; 

	/* Inicjalizacja generatora liczb losowych */
	srand(time(0));

	wnd = initscr();
	start_color();
	
	if(has_colors()) {

		enable_tile_colors();
		use_default_colors();

		/* Przyporządkuj każdej płytce kolor */
		init_pair(1, COLOR_YELLOW, -1);
		init_pair(2, COLOR_GREEN, -1);
		init_pair(3, COLOR_RED, -1);
		init_pair(4, COLOR_YELLOW, -1);
		init_pair(5, COLOR_WHITE, -1);
		init_pair(6, COLOR_RED, -1);
		init_pair(7, COLOR_GREEN, -1);
		init_pair(8, COLOR_BLUE, -1);
		init_pair(9, COLOR_CYAN, -1);
		init_pair(10, COLOR_MAGENTA, -1);

	}
	else {

		disable_tile_colors();

	}

	cbreak();
	echo();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	getmaxyx(wnd, ysize, xsize);

	/* Pobierz docelowy rozmiar */
	while(1) {

		werase(wnd);

		draw_frame(wnd);

		wmove(wnd, ysize/2-(MAX_SIZE-MIN_SIZE+2), 20);	

		wprintw(wnd, "Available pool size:");

		for(i=MIN_SIZE;i<=MAX_SIZE;i++) {

			wmove(wnd, ysize/2-(MAX_SIZE-MIN_SIZE+2)+(i-MIN_SIZE+1), 20);
			attron(COLOR_PAIR(5) | A_BOLD);
			wprintw(wnd, "%d", i);
			attroff(COLOR_PAIR(5) | A_BOLD);
			wprintw(wnd, " for %dx%d", i, i);
			
		}

		wmove(wnd, ysize-2, 2);
		wprintw(wnd, "Saephir (c), 2010");

		wmove(wnd, ysize/2+1, 20);
		wprintw(wnd, "Type the desired number: ");

		c = getch();

		buf[0] = c;
		buf[1] = 0;

		i2 = atoi(buf);

		if(i2 >= MIN_SIZE && i2 <= MAX_SIZE) {
			TILES_COUNT = i2;
			init();
			break;
		}

	}

	while(1) {

		werase(wnd);

		draw_frame(wnd);

		/* Wygrana gra */
		if(verify()) {

			print_game(wnd);

			wmove(wnd, ysize/2, 40);
			wprintw(wnd, "Well done!");
			wmove(wnd, ysize/2+1, 40);
			wprintw(wnd, "Press ENTER to exit...");
			getch();
			break;

		}

		print_game(wnd);
		print_pool(wnd);

		wmove(wnd, ysize-2, 1);

		wgetnstr(wnd, buf, 49);
		buf[49] = 0;

		if(strcmp(buf, "exit") == 0 || strcmp(buf, "quit") == 0 || strcmp(buf, "q") == 0) {

			break;

		}
		else if(strcmp(buf, "help") == 0 || strcmp(buf, "?") == 0) {

			wmove(wnd, ysize-2, 1);
			wclrtoeol(wnd);
			draw_frame(wnd);
			wmove(wnd, ysize-2, 1);
			wprintw(wnd, HELP_LINE);
			getch();
			wmove(wnd, ysize-2, 1);
			wclrtoeol(wnd);
			draw_frame(wnd);
			wmove(wnd, ysize-2, 1);
			wprintw(wnd, HELP_LINE2);
			getch();

		}

		else if(strcmp(buf, "<<") == 0) {

			push_left();

		}

		else if(strcmp(buf, ">>") == 0) {

			push_right();

		}

		else if(strcmp(buf, "^^") == 0) {

			push_top();

		}

		else if(strcmp(buf, "vv") == 0) {

			push_bottom();

		}
		else if(sscanf(buf, "%c%d<%c%d", &c, &i, &c2, &i2) == 4) {

			i--;
			i2--;

			if(c == c2 && i == i2)
				continue;

			if(c-'a' < 0 || c-'a' >= 2*TILES_COUNT)
				continue;

			if(c2-'a' < 0 || c2-'a' >= 2*TILES_COUNT)
				continue;

			if(i < 0 || i >= TILES_COUNT)
				continue;

			if(i2 < 0 || i2 >= TILES_COUNT)
				continue;

			/* Przesuwamy na pole gry */
			if(c-'a' < TILES_COUNT) {

				temp_tile = &game[i][c-'a'];

				/* Miejsce musi być puste */
				if(!is_empty(temp_tile))
					continue;

				/* przesuwamy z puli do gry */
				if(c2-'a' >= TILES_COUNT)
					temp_tile = &pool[i2][c2-'a'-TILES_COUNT];
				/* przesuwamy z gry do gry */
				else
					temp_tile = &game[i2][c2-'a'];

				/* Żródło nie może być puste */
				if(is_empty(temp_tile))
					continue;

				move_tile(&game[i][c-'a'], temp_tile);

			}
			/* Przesuwamy do puli */
			else {

				temp_tile = &pool[i][c-'a'-TILES_COUNT];

				/* Miejsce musi być puste */
				if(!is_empty(temp_tile))
					continue;

				/* Przesuwamy z pola gry do puli */
				if(c2-'a' < TILES_COUNT)
					temp_tile = &game[i2][c2-'a'];
				/* Przesuwamy z puli do puli */
				else
					temp_tile = &pool[i2][c2-'a'-TILES_COUNT];
				
				/* Żródło nie może być puste */
				if(is_empty(temp_tile))
					continue;

				move_tile(&pool[i][c-'a'-TILES_COUNT], temp_tile);

			}
			


		}
		else if(sscanf(buf, "%c%d>%c%d", &c, &i, &c2, &i2) == 4) {

			i--;
			i2--;

			if(c == c2 && i == i2)
				continue;

			if(c-'a' < 0 || c-'a' >= 2*TILES_COUNT)
				continue;

			if(c2-'a' < 0 || c2-'a' >= 2*TILES_COUNT)
				continue;

			if(i < 0 || i >= TILES_COUNT)
				continue;

			if(i2 < 0 || i2 >= TILES_COUNT)
				continue;

			/* Przesuwamy do puli */
			if(c2-'a' >= TILES_COUNT) {

				temp_tile = &pool[i2][c2-'a'-TILES_COUNT];

				/* Miejsce musi być puste */
				if(!is_empty(temp_tile))
					continue;

				/* Przesuwamy z pola gry */
				if(c-'a' < TILES_COUNT)
					temp_tile = &game[i][c-'a'];
				/* Przesuwamy z puli */
				else
					temp_tile = &pool[i][c-'a'-TILES_COUNT];
				
				/* Żródło nie może być puste */
				if(is_empty(temp_tile))
					continue;

				move_tile(&pool[i2][c2-'a'-TILES_COUNT], temp_tile);

			}
			/* Przesuwamy na pole gry */
			else {

				temp_tile = &game[i2][c2-'a'];

				/* Miejsce musi być puste */
				if(!is_empty(temp_tile))
					continue;

				/* Przesuwamy z puli na pole gry */
				if(c-'a' >= TILES_COUNT)
					temp_tile = &pool[i][c-'a'-TILES_COUNT];
				/* Przesuwamy z pola gry na pole gry */
				else
					temp_tile = &game[i][c-'a'];
				
				/* Żródło nie może być puste */
				if(is_empty(temp_tile))
					continue;

				move_tile(&game[i2][c2-'a'], temp_tile);

			}

		}
		else if(sscanf(buf, "%c%d>pool", &c, &i) == 2 || sscanf(buf, "pool<%c%d", &c, &i) == 2) {

			i--;

			if(strcmp(&buf[2], ">pool") != 0) {
				buf[5] = 0;
				if(strcmp(buf, "pool<") != 0)
					continue;
			}

			if(c-'a' < 0 || c-'a' >= TILES_COUNT)
				continue;

			if(i < 0 || i >= TILES_COUNT)
				continue;

			/* Żródło nie może być puste */
			if(is_empty(&game[i][c-'a']))
				continue;

			/* Jeżeli źródło nie jest puste, to istnieją wolne miejsca w puli */
			/* znajdujemy więc pierwsze wolne miejsce */
			for(y=0;y<TILES_COUNT;y++) {
				for(x=0;x<TILES_COUNT;x++) {
					
					if(is_empty(&pool[x][y])) {
						
						move_tile(&pool[x][y], &game[i][c-'a']);
						break;
					}

				}
			}
			

		}

	}

	endwin();

	return 0;
}
