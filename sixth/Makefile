all: rule

rule: sixth.c
		gcc -Wall -Werror -fsanitize=address,undefined -g sixth.c -o sixth
clean:
		rm -rf sixth