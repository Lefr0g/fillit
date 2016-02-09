/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/02/02 17:01:16 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** save actual env (e) agencement ( fixed tetriminos ) to a printable str
** Steps :
**	- last char to '\0'
**	- from (last char - 1), all ( map size + 1) chars, set a '\n'
**	- then browse the tetrimino list and for each fixed tetriminos
** !!! PARAMETER &MAP _MUST_ BE AT THE GOOD SIZE,
** AND THIS SIZE MUST BE BASED ON E->SMALLEST_SIZE !!!
*/

void	fillit_save_printable(t_env *e, char **map)
{
	char	*ret;
	int		sq_siz;
	int		c;
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	l_ptr = e->first;
	t_ptr = (t_tetri *)l_ptr->content;
	ret = *map;
	sq_siz = e->smallest_size;
	ft_memset(ret, '.', sq_siz * (sq_siz + 1));
	while (l_ptr)
	{
		c = 0;
		while (c++ < 4)
			ret[t_ptr->x_offset - e->xmin + t_ptr->x[c - 1] + \
				((t_ptr->y[c - 1] + t_ptr->y_offset - e->ymin) * \
				 (sq_siz + 1))] = t_ptr->letter;
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri *)l_ptr->content;
	}
	c = sq_siz * (sq_siz + 1) - 1;
	while ((c -= sq_siz + 1) > 0)
		ret[c] = '\n';
	ret[e->smallest_size * (e->smallest_size + 1) - 1] = 0;
}

void	fillit_print_colored(char *map)
{
	int		i;
	int		color;

	i = 0;
	while (map[i])
	{
		ft_putstr("\x1b[0m");
		if (ft_isalpha(map[i]))
		{
			color = map[i] - 24;
			ft_putstr("\x1b[");
			ft_putnbr(color);
			ft_putchar('m');
		}
		ft_putchar(map[i]);
		i++;
	}
	ft_putstr("\x1b[0m");
}

/*
** This funtion printfs a standard or specified error message on the error
** output.
** -------------------------- To be added to libft ----------------------------
*/

int		fillit_error(char *str, int mode)
{
	if (!str || !mode)
	{
		ft_putstr("error");
		ft_putstr("\n");
	}
	else
	{
		ft_putstr_fd("error : ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
	}
	if (!DEBUG_MODE)
		exit(0);
	return (-1);
}

char	*fillit_get_output_map(t_env *e)
{
	t_vars	v;
	int		i;
	int		side;
	char	*out;

	fillit_init_vars(&v);
	fillit_get_fixed_range(e, &v);
	if (v.xmax - v.xmin > v.ymax - v.ymin)
		side = v.xmax - v.xmin + 1;
	else
		side = v.ymax - v.ymin + 1;

	printf("get_output_map() : Allocating memory...");

	out = ft_strnew((side + 1) * (side) + 1);
	ft_memset(out, '.', (side + 1) * (side) + 1);
	
	printf(" DONE\n");

//	out[(side + 1) * side + 1] = '\0';

//	printf("ymin = %d, ymax = %d\n", v.ymin, v.ymax);
	
	i = side;
	while (i < (side + 1) * side)
	{
		out[i] = '\n';
		i += side + 1;
	}
	i -= side;
	out[i] = '\0';


	v.lst_ptr = e->first;
	while (v.lst_ptr)
	{
		v.tet_ptr = (t_tetri*)v.lst_ptr->content;
//		/*
		i = 0;
		while (i < 4 && v.tet_ptr->fixed)
		{
			out[(v.tet_ptr->x[i] + v.tet_ptr->x_offset - v.xmin)
				+ ((v.tet_ptr->y[i] + (v.tet_ptr->y_offset) - v.ymin) * (side + 1))]
				= v.tet_ptr->letter;
			i++;
		}
//		*/
		v.lst_ptr = v.lst_ptr->next;
	}
//	printf("End of fillit_get_output_map()\n");
	return (out);
}
