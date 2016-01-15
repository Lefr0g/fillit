#!/bin/sh

make -C ~/42/libft
clang -Wall -Werror -Wextra -I ~/42/libft/includes -lft -L ~/42/libft main.c -o run.out
