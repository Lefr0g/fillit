/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/01/11 18:25:14 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function prints the tetrimino passed as parameter on stdout, based on
** its xy coordinates, NOT on the raw string.
** The two last conditions of the main loop avoid printing unallocated
** coordinate tables when debugging.
*/

void	fillit_print_single_tetri(t_tetri *ptr)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	x = 0;
	y = 0;
	while (i < 4 && ptr->x[i] < 4 && ptr->y[i] < 4)
	{
		while (x < ptr->x[i])
		{
			ft_putchar(' ');
			x++;
		}
		ft_putchar(ptr->letter);
		x++;
		if (y != ptr->y[i + 1])
		{
			ft_putchar('\n');
			x = 0;
			y++;
		}
		i++;
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
	char	tbd;
	int		c;
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	c = e->smallest_size;
	l_ptr = e->first;
	t_ptr = (t_tetri *)l_ptr->content;
	ret = (char *)malloc(c * (c + 1) + 1);
	ft_memset(ret, '.', c * (c + 1));
	ret[c * (c + 1) + 1] = 0;
	c = c * (c + 1) - 1;
	while (c > 0)
	{
		ret[c] = '\n';
		c -= e->smallest_size + 1;
	}
	while (l_ptr)
	{
		printf(">>> DEBUG - fillit_save_printable() running on tetri %c\n", t_ptr->letter);
		if (t_ptr->fixed)
		{
			c = 0;
			while (c++ < 4)
			{
				ret[t_ptr->x_offset + t_ptr->x[c - 1] + \
					((t_ptr->y[c - 1] + t_ptr->y_offset) * \
					 (e->smallest_size + 1))] = t_ptr->letter;
				tbd = ret[t_ptr->x_offset + t_ptr->x[c - 1] + \
					((t_ptr->y[c - 1] + t_ptr->y_offset) * \
					 (e->smallest_size + 1))];
				printf(">>> DEBUG : tbd = %c\n", tbd);
			}
		}
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri *)l_ptr->content;
	}
	return (ret);
}
