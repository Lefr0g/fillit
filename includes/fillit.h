/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes/fillit.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:38 by amulin            #+#    #+#             */
/*   Updated: 2016/01/20 18:26:10 by lpoujade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLIT_H
# define FILLIT_H

# include "libft.h"
# include <fcntl.h>

# include <stdio.h>

typedef struct		s_tetri
{
	char			letter;
	char			raw[17];
	int				x[4];
	int				y[4];
	int				order[4];
	int				x_offset;
	int				y_offset;
	int				fixed;
	int				firstmove : 1;
}					t_tetri;

typedef struct		s_tmp
{
	int				i;
	int				fd;
	int				gnl_ret;
	int				jump;
	int				layers;
	int				layercheck_ret;
	int				blocks;
	char			*line;

}					t_tmp;

typedef struct		s_vars
{
	int				height;
	int				pos;
	int				i;
	int				isaved;
	int				xmax;
	int				xmin;
	int				ymax;
	int				ymin;
	t_list			*lst_ptr;
	t_list			*lst_mov;
	t_list			*lst_fix;
	t_tetri			*tet_ptr;
	t_tetri			*tet_mov;
	t_tetri			*tet_fix;
}					t_vars;

typedef struct		s_env
{
	t_tmp			*tmp;
	t_list			*first;
	int				i;
	int				j;
	int				x;
	int				y;
	int				xmin;
	int				ymin;
	int				smallest_size; // cote du plus petit carre
	size_t			tcount; // nombre de tetris dans la liste
	size_t			tlocked; // nombre de tetris verouilles en position
	char			set[25]; // Stockage des lettres a ne pas deplacer
	char			*result; // Map de sortie
	int				block;
	int				letter;
	int				inception;
	int				update : 1;
	int				color : 1;
}					t_env;

/*
** main.c
*/
int		fillit_error(char *str);
void	fillit_free_all(t_env *e);
void	fillit_reset_quickvars(t_env *e);
void	fillit_reset_tmp(t_tmp *tmp);

/*
** fillit_structs.c
*/
int		fillit_init(t_env **e);
void	fillit_reset_tmp(t_tmp *tmp);
void	fillit_reset_quickvars(t_env *e);
void	fillit_init_vars(t_vars *t);
void	fillit_del_tetri(void *content, size_t size);
void	fillit_free_all(t_env *e);

/*
** fillit_parse.c
*/
int		fillit_parse(t_env *e, char *filename);
int		fillit_input_check(t_env *e);
int		fillit_layer_check(t_tmp *tmp, t_list **list_ptr, t_tetri **tetri_ptr);
int		fillit_blocks_check(t_env *e, t_tetri *tetri_ptr);
int		fillit_new_tetri(t_list **list_ptr, t_tetri **tetri_ptr);

/*
** fillit_calc.c
*/
int		fillit_calc(t_env *start);
int		fillit_square_size(t_env *e);
void	fillit_square_get_range(t_vars *v);
void	fillit_load_xy(t_env *e);
void	fillit_xy_get(t_tetri *t_ptr);
void	fillit_xy_correct(t_tetri *ptr);
void	fillit_order_get(t_tetri *ptr);
void	fillit_order_get_bottom_up(t_vars *v, t_tetri *ptr);

/*
** fillit_print.c
*/
void	fillit_print_raw(t_env *e);
void	fillit_print_xy(t_tetri *ptr);
void	fillit_print_single_tetri(t_tetri *ptr);
void	fillit_save_printable(t_env *e, char **map);
void	fillit_print_colored(char *map);

/*
** fillit_position.c
*/
int		fillit_check_collision(t_env *e, t_tetri *moving);
int		fillit_xy_collision(int x, int y, t_tetri *ptr);
void	fillit_move_around(t_env *e);
void	fillit_move_and_try(t_env *e, t_tetri *moving, int x, int y);
void	debug_inception_print(t_env *e);

#endif
