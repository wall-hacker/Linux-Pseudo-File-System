all: build

build:
	gcc -Wall -Wextra -std=c99 -g main.c tree.c aux_tree.c list.c -o sd_fs

clean:
	rm *.o sd_fs

run:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./sd_fs
