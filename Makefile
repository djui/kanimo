CC=gcc

all: normal obfuscated

normal:
	$(CC) -o bin/kanim src/kanim.c

obfuscated:
	$(CC) -o bin/kanimo src/kanim_obfuscated.c
