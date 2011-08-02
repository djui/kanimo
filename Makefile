CC=gcc
FLAGS=-std=c99 -O3

all: normal obfuscated

normal:
	$(CC) $(FLAGS) -o bin/kanim src/kanim.c

obfuscated:
	$(CC) $(FLAGS) -o bin/kanimo src/kanim_obfuscated.c
