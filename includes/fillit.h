
#ifndef FILLIT_H
# define FILLIT_H

# include "libft.h"
# include <fcntl.h>

# include <stdio.h>

typedef struct		s_tmp
{
	int				fd;
	int				i;
	int				j;
	int				gnl_ret;
	int				linecheck_ret;
	int				jump;
	char			*line;
}					t_tmp;

typedef struct		s_tetri
{
	char			letter;
	char			*raw;
	int				x[4];
	int				y[4];
}					t_tetri;

typedef struct		s_env
{
	t_tmp			*tmp;
	t_list			*first;
}					t_env;

/*
** main.c
*/
int		fillit_error(char *str);
void	fillit_free_all(t_env *e);

/*
** fillit_parse.c
*/
int		fillit_parse(t_env *e, char *filename);
int		fillit_input_check(t_env *e, int fd);
int		fillit_line_check(t_tmp *tmp);

#endif
