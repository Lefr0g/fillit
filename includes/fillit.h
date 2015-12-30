/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:38 by amulin            #+#    #+#             */
/*   Updated: 2015/12/30 19:22:16 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLIT_H
# define FILLIT_H

# include "libft.h"
# include <fcntl.h>

# include <stdio.h>

typedef struct		s_tmp
{
	int				fd;
	int				i;
	int				gnl_ret;
	int				jump;
	int				layers;
	int				layercheck_ret;
	int				blocks;
	char			*line;
}					t_tmp;

typedef struct		s_tetri
{
	char			letter;
	char			raw[17];
	int				x[4];
	int				y[4];
}					t_tetri;

typedef struct		s_env
{
	t_tmp			*tmp;
	t_list			*first;
	int				i;
	int				j;
	int				x;
	int				y;
	int				block;
	int				letter;
}					t_env;

/*
** main.c
*/
int		fillit_error(char *str);
void	fillit_free_all(t_env *e);
void	fillit_reset_quickvars(t_env *e);

/*
** fillit_parse.c
*/
int		fillit_parse(t_env *e, char *filename);
int		fillit_input_check(t_env *e);
int		fillit_layer_check(t_tmp *tmp, t_list **list_ptr, t_tetri **tetri_ptr);
int		fillit_blocks_check(t_env *e, t_tetri *tetri_ptr);
int		fillit_new_tetri(t_list **list_ptr, t_tetri **tetri_ptr);

/*
** calc.c
*/
int		fillit_calc(t_env *start);
void	fillit_load_xy(t_env *e);
void	fillit_xy_get(t_env *e, t_tetri *t_ptr);

/*
** fillit_print.c
*/
void	fillit_print_raw(t_env *e);
void	fillit_print_xy(t_tetri *ptr);

#endif
