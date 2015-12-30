/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2015/12/30 18:41:53 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int		calc(t_env *start)
{
	t_list	*p;

	p = start->first;
	while (p)
	{
		//solve;
		p = p->next;
	}
	return (0);
}

/*
** The following function transforms the 'raw' string into 'x' and 'y' int
** tables for each tetrimino, one per list element.
*/

int		fillit_load_xy(t_env *e)
{
	char	letter;
	int		block;
	int		i;
	int		j;
	int		x;
	int		y;
	t_tetri	*t_ptr;
	t_list	*l_ptr;

	letter = 'A';
	block = 0;
	i = 0;
	j = 0;
	x = 0;
	y = 0;
	l_ptr = e->first;
	t_ptr = (t_tetri*)l_ptr->content;
	while (l_ptr)
	{
		t_ptr->letter = letter;
		while (t_ptr->raw[i] && block < 4)
		{
			if (t_ptr->raw[i] == '#')
			{
				if (!j)
					(void)j;
				else if (i - j == 1)
					x++;
				else if (i - j == 2)
				{
					x -= 2;
					y++;
				}
				else if (i - j == 3)
				{
					x--;
					y++;
				}
				else if (i - j == 4)
					y++;
				else
					return (fillit_error("load_xy : incoherent spacing between blocks"));
				t_ptr->x[block] = x;
				t_ptr->y[block] = y;
				block++;
				j = i;
			}
			i++;
		}
		l_ptr = l_ptr->next;
		if (l_ptr)
			t_ptr = (t_tetri*)l_ptr->content;
		letter++;
		block = 0;
		i = 0;
		j = 0;
		x = 0;
		y = 0;
	}
	return (0);
}
