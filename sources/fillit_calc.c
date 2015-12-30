/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2015/12/30 19:22:07 by amulin           ###   ########.fr       */
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

void	fillit_xy_get(t_env *e, t_tetri *t_ptr)
{
	if (!e->j)
		(void)e->j;
	else if (e->i - e->j == 1)
		e->x++;
	else if (e->i - e->j == 2)
	{
		e->x -= 2;
		e->y++;
	}
	else if (e->i - e->j == 3)
	{
		e->x--;
		e->y++;
	}
	else if (e->i - e->j == 4)
		e->y++;
	else
		fillit_error("load_xy : incoherent spacing between blocks");
	t_ptr->x[e->block] = e->x;
	t_ptr->y[e->block] = e->y;
}

/*
** The following function transforms the 'raw' string into 'x' and 'y' int
** tables for each tetrimino, one per list element.
*/

void	fillit_load_xy(t_env *e)
{
	t_tetri	*t_ptr;
	t_list	*l_ptr;

	e->letter = 'A';
	fillit_reset_quickvars(e);
	l_ptr = e->first;
	t_ptr = (t_tetri*)l_ptr->content;
	while (l_ptr)
	{
		t_ptr->letter = e->letter;
		while (t_ptr->raw[e->i] && e->block < 4)
		{
			if (t_ptr->raw[e->i] == '#')
			{
				fillit_xy_get(e, t_ptr);
				e->block++;
				e->j = e->i;
			}
			e->i++;
		}
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
}
