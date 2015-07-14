tetravex: tetravex.o tile.o print.o
	gcc -lncurses -o tetravex tile.o print.o tetravex.o
tetravex.o: tetravex.c
	gcc -O2 -c -o tetravex.o tetravex.c
tile.o: tile.c
	gcc -O2 -c -o tile.o tile.c
print.o: print.c
	gcc -O2 -c -o print.o print.c
