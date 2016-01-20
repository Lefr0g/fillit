/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/01/20 18:30:18 by lpoujade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function prints the tetrimino passed as parameter on stdout, based on
** its xy coordinates, NOT on the raw string.
*/

void	fillit_print_single_tetri(t_tetri *ptr)
{
	int	x;
	int	y;
	int	block;

	x = 0;
	y = 0;
	block = 0;
	while (y < 4 && block < 4)
	{
		while (x < 4 && block < 4)
		{
			if (ptr->y[block] == y && ptr->x[block] == x)
			{
				ft_putchar(ptr->letter);
				block++;
			}
			else
				ft_putchar(' ');
			x++;
		}
		y++;
		x = 0;
		ft_putchar('\n');
	}
}
	
/*
** Printing debug subfunction.
*/

void	fillit_print_xy(t_tetri *ptr)
{
	int	i;
	
	i = 0;
	ft_putstr("\t\t     x -> |");
	while (i < 4)
	{
		if (ptr->x[i] >= 0)
			ft_putchar(' ');
		ft_putnbr(ptr->x[i]);
		ft_putchar('|');
		i++;
	}
	ft_putchar('\n');
	i = 0;
	ft_putstr("\t\t     y -> |");
	while (i < 4)
	{
		if (ptr->y[i] >= 0)
			ft_putchar(' ');
		ft_putnbr(ptr->y[i]);
		ft_putchar('|');
		i++;
	}
	ft_putchar('\n');
	i = 0;
	ft_putstr("\t\t order -> |");
	while (i < 4)
	{
		ft_putchar(' ');
		ft_putnbr(ptr->order[i]);
		ft_putchar('|');
		i++;
	}
	ft_putchar('\n');
}

/*
** Parent printing debug function.
*/

void	fillit_print_raw(t_env *e)
{
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	l_ptr = e->first;
	ft_putendl("\n================ DEBUG ================");
	while (l_ptr)
	{
		t_ptr = (t_tetri*)l_ptr->content;
		ft_putstr("Tetrimino ");
		ft_putchar(t_ptr->letter);
		ft_putstr(" contains : ");
		ft_putendl(t_ptr->raw);
		l_ptr = l_ptr->next;
		fillit_print_xy(t_ptr);
		fillit_print_single_tetri(t_ptr);
		ft_putchar('\n');
	}
}

/*
** save actual env (e) agencement ( fixed tetriminos ) to a printable str
** Steps :
**	- last char to '\0'
**	- from (last char - 1), all ( map size + 1) chars, set a '\n'
**	- then browse the tetrimino list and for each fixed tetriminos
** !!! PARAMETER &MAP _MUST_ BE AT THE GOOD SIZE , AND THIS SIZE MUST BE BASED ON E->SMALLEST_SIZE !!!
*/

void	fillit_save_printable(t_env *e, char **map)
{
	char	*ret;
	int		c;
	int		sq_siz;
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
	c = sq_siz;
	ret[c * (c + 1) + 1] = 0;
	c = c * (c + 1) - 1;
	while (c > 0)
	{
		ret[c] = '\n';
		c -= sq_siz + 1;
	}
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
