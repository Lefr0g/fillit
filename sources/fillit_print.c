/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2015/12/30 18:37:20 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

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

void	fillit_print_raw(t_env *e)
{
	int		i;
	t_list	*l_ptr;
	t_tetri	*t_ptr;

	l_ptr = e->first;
	i = 0;
	ft_putendl("\n================ DEBUG ================");
	while (l_ptr)
	{
		t_ptr = (t_tetri*)l_ptr->content;
		ft_putstr("Tetrimino ");
		ft_putnbr(i);
		ft_putstr(" contains : ");
		ft_putendl(t_ptr->raw);
		l_ptr = l_ptr->next;
		i++;
		fillit_print_xy(t_ptr);
	}
}
