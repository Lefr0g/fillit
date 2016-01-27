/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/01/26 14:59:59 by amulin           ###   ########.fr       */
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
