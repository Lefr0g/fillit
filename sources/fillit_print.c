/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/01/14 15:53:36 by amulin           ###   ########.fr       */
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
**	- reserve mem space of ( map size ) * ( map size + 1: \n) + 1: \0
**	- memset with '.'
**	- last char to '\0'
**	- from (last char - 1), all ( map size + 1) chars, set a '\n'
**	- then browse the tetrimino list and for each fixed tetriminos
*/

char	*fillit_save_printable(t_env *e)
{
	char	*ret;
	int		c;
	int		sq_siz;
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	sq_siz = fillit_square_size(e);
	l_ptr = e->first;
	t_ptr = (t_tetri *)l_ptr->content;
	ret = (char *)malloc(sq_siz * (sq_siz + 1) + 1);
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
	ft_putnbr(fillit_square_size(e));ft_putchar('\n');
	return (ret);
}
