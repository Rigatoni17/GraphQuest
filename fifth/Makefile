all: rule

rule: fifth.c
		gcc -Wall -Werror -fsanitize=address,undefined -g fifth.c -o fifth
clean:
		rm -rf fifth