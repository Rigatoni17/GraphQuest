all: rule

rule: second.c
		gcc -Wall -Werror -fsanitize=address,undefined -g second.c -o second
clean:
		rm -rf second