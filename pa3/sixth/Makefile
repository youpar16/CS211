all: sixth

sixth: sixth.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 sixth.c -o sixth -lm

clean:
	rm -rf sixth