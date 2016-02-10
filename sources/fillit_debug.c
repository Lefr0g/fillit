/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:51:24 by amulin            #+#    #+#             */
/*   Updated: 2016/02/10 14:51:12 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function prints one '>' per level of deepness reached into recursivity.
*/

void	debug_inception_print(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->inception)
	{
		ft_putstr("\033[33m> \033[0m");
		i++;
	}
}

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
	ft_putstr("\n\t\t    x_offset = ");
	ft_putnbr(ptr->x_offset);
	ft_putstr("\n\t\t    y_offset = ");
	ft_putnbr(ptr->y_offset);
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
		fillit_print_xy(t_ptr);
		fillit_print_single_tetri(t_ptr);
		ft_putchar('\n');
		l_ptr = l_ptr->next;
	}
}

void	fillit_print_all_tetri_status(t_env *e)
{
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	l_ptr = e->first;
	while (l_ptr)
	{
		t_ptr = (t_tetri*)l_ptr->content;
		printf("Tetri %c is ", t_ptr->letter);
		if (t_ptr->fixed)
			printf("fixed (%d)\n", t_ptr->fixed);
		else
			printf("moving\n");
		printf("Its offsets are %d (x) and %d (y)\n", t_ptr->x_offset,
				t_ptr->y_offset);
		l_ptr = l_ptr->next;
	}
}
