#!/bin/sh

rm run.out
make -C ../libft
clang -Wall -Werror -Wextra -I ../libft/includes -lft -L ../libft main.c -o run.out
