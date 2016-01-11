/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/01/11 17:40:27 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function ensures all the x coordinates are positive by changing the x
** reference if needed.
*/

void	fillit_x_correct(t_tetri *ptr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		if (ptr->x[i] < j)
			j = ptr->x[i];
		i++;
	}
	if (j < 0)
	{
		i = 0;
		while (i < 4)
		{
			ptr->x[i] += -j;
			i++;
		}
	}
}

/*
** In this function, e->i is the position of the current block within the raw
** string, and e->j is the position of the previous block.
** Based on the fact that all tetrimino have known shape limitations, this
** function translates the current block position into xy coordinates, based on
** the know data on the previous block and the spacing witch separates them.
*/

void	fillit_xy_get(t_env *e, t_tetri *t_ptr)
{
	if (e->i - e->j == 1)
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
	else if (!e->j)
		(void)e->j;
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
	e->tcount = 0;
	e->tlocked = 1;
	fillit_reset_quickvars(e);
	l_ptr = e->first;
	t_ptr = (t_tetri*)l_ptr->content;
	while (l_ptr && e->letter <= 'Z')
	{
		t_ptr->letter = e->letter;
		e->tcount++;
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
		fillit_x_correct(t_ptr);
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
}
