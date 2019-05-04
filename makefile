make:
	@gcc -o main main.c -g -Wall -B -lm -std=c99 -pedantic
	@./main