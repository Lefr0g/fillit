# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amulin <amulin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/28 16:48:40 by amulin            #+#    #+#              #
#    Updated: 2016/02/12 17:02:05 by amulin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fillit

CC = clang

FLAGS = -Wall -Werror -Wextra

SRCS = main.c fillit_parse.c fillit_print.c fillit_calc.c fillit_position.c \
	   fillit_structs.c fillit_solve.c fillit_debug.c ft_basicfilecheck.c \
	   fillit_input_checks.c

SRCSDIR = sources/

SOURCES = $(addprefix $(SRCSDIR), $(SRCS))

HDRS = fillit.h

HEADIR = includes/

HEADERS = $(addprefix $(HEADIR), $(HDRS))

INCLUDES = -I $(HEADIR)

LIBFLAG = -lft

LIBHDRS = includes/

LIBDIR = libft/

LIBDIRFLAG = -L $(LIBDIR)

LIBHEADIR = $(addprefix $(LIBDIR), $(LIBHDRS))

LIBINCLUDES = -I $(LIBHEADIR)

LIBNAME = libft.a

LIBFULL = $(addprefix $(LIBDIR), $(LIBNAME))

all : lib $(NAME)

$(NAME) : $(LIBFULL) $(SOURCES) $(HEADERS)
	$(CC) $(FLAGS) $(SOURCES) $(INCLUDES) $(LIBINCLUDES) $(LIBDIRFLAG) \
		$(LIBFLAG) -o $(NAME)

$(LIBFULL) : lib

lib :
	make -C libft/

clean :
	make -C libft/ clean

fclean : clean
	make -C libft/ fclean
	-rm $(NAME)

re : fclean all

.PHONY: all clean fclean re
