CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
 
SRC_COMMUN = src/jeu.c src/arbre.c
 
all: puissance4 
 
puissance4: src/main.c src/jeu.c $(SRC_COMMUN)
	$(CC) $(CFLAGS) -o $@ $^
 
test_arbre: src/test_arbre.c $(SRC_COMMUN)
	$(CC) $(CFLAGS) -o $@ $^
 
clean:
	rm -f puissance4 test_arbre *.o