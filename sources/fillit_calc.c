/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/01/20 15:19:50 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This is a subfunction for fillit_order_get, scanning tetri from the bottom
** to the top and ordering the blocks consequently.
*/

void	fillit_order_get_bottom_up(t_tmp *tmp, t_tetri *tet)
{
	while (tmp->height >= 0)
	{
		tmp->xmax = INT_MIN;
		tmp->i = -1;
		while ((tmp->i)++ < 4)
			if (tet->y[tmp->i] == tmp->height && tet->x[tmp->i] > tmp->xmax)
				tmp->xmax = tet->x[tmp->i];
		tmp->i = -1;
		while ((tmp->i)++ < 4)
			if (tet->y[tmp->i] == tmp->height && tet->x[tmp->i]
					== tmp->xmax - 1)
			{
				tet->order[tmp->pos] = tmp->i;
				tmp->pos++;
				tmp->xmax--;
				tmp->i = -1;
			}
		tmp->height--;
	}
}

/*
** This function finds out the order of the blocks, so we can rotate
** around the tetri clockwise, starting top-left of fixed tetri, according to
** the rules defined in the subject.
** The result will be stored in a int[4], within each t_tetri.
** This data will be used in the high-level loop of the fillit_move_around()
** function instead of the simple i increment.
*/

void	fillit_order_get(t_tmp *tmp, t_tetri *tet)
{
	fillit_reset_tmp(tmp);
	tmp->ymax = ft_tabmax(tet->y, 4);
	while (tmp->height++ <= tmp->ymax)
	{
		tmp->i = -1;
		tmp->xmax = INT_MIN;
		while ((tmp->i)++ < 4)
			if (tet->y[tmp->i] == tmp->height && tet->x[tmp->i] > tmp->xmax)
			{
				tmp->xmax = tet->x[tmp->i];
				tmp->isaved = tmp->i;
			}
		if (tmp->isaved != -1)
		{
			tet->order[tmp->pos] = tmp->isaved;
			(tmp->pos)++;
			tmp->isaved = -1;
		}
	}
	fillit_order_get_bottom_up(tmp, tet);
}

/*
** This function ensures all the x coordinates are positive by changing the x
** reference if needed.
*/

void	fillit_xy_correct(t_tetri *ptr)
{
	int	i;
	int	min;

	min = ft_tabmin(ptr->x, 4);
	if (min > 0)
	{
		i = 0;
		while (i < 4)
		{
			ptr->x[i] -= min;
			i++;
		}
	}
	min = ft_tabmin(ptr->y, 4);
	if (min > 0)
	{
		i = 0;
		while (i < 4)
		{
			ptr->y[i] -= min;
			i++;
		}
	}
}

/*
** This function transforms the raw table into a double table of
** coordinates (x,y)
*/

void	fillit_xy_get(t_tetri *t_ptr)
{
	int	i;
	int	j;
	int	r;
	int	block;

	i = 0;
	j = 0;
	r = 0;
	block = 0;
	while (j < 4)
	{
		while (i < 4)
		{
			if (t_ptr->raw[r] == '#')
			{
				t_ptr->x[block] = i;
				t_ptr->y[block] = j;
				block++;
			}
			i++;
			r++;
		}
		j++;
		i = 0;
	}
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
		fillit_xy_get(t_ptr);
		fillit_xy_correct(t_ptr);
		fillit_order_get(e->tmp, t_ptr);
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
	if (e->letter == 'Z')
		fillit_error("too many tetriminos, 25 max.");
}
