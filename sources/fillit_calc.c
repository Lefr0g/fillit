/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/01/28 15:16:59 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This is a subfunction for fillit_order_get, scanning tetri from the bottom
** to the top and ordering the blocks consequently.
*/

void	fillit_order_get_bottom_up(t_vars *v, t_tetri *tet)
{
	while (v->height >= 0)
	{
		v->xmax = INT_MIN;
		v->i = -1;
		while (++(v->i) < 4)
			if (tet->y[v->i] == v->height && tet->x[v->i] > v->xmax)
				v->xmax = tet->x[v->i];
		v->i = -1;
		while (++(v->i) < 4)
			if (tet->y[v->i] == v->height && tet->x[v->i]
					== v->xmax - 1)
			{
				tet->order[v->pos] = v->i;
				v->pos++;
				v->xmax--;
				v->i = -1;
			}
		v->height--;
	}
}

/*
** This function finds out the order of the blocks, so we can rotate
** around the tetri clockwise, starting top-left of fixed tetri, according to
** the rules defined in the subject.
** The result will be stored in a int[4], within each t_tetri.
** This data will be used in the high-level loop of the fillit_solve()
** function instead of the simple i increment.
*/

void	fillit_order_get(t_tetri *tet)
{
	t_vars	v;

	fillit_init_vars(&v);
	v.ymax = ft_tabmax(tet->y, 4);
	while (v.height++ <= v.ymax)
	{
		v.i = -1;
		v.xmax = INT_MIN;
		while (++(v.i) < 4)
			if (tet->y[v.i] == v.height && tet->x[v.i] > v.xmax)
			{
				v.xmax = tet->x[v.i];
				v.isaved = v.i;
			}
		if (v.isaved != -1)
		{
			tet->order[v.pos] = v.isaved;
			(v.pos)++;
			v.isaved = -1;
		}
	}
	fillit_order_get_bottom_up(&v, tet);
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
	e->tcount = 1;
	e->tlocked = 0;
	fillit_reset_quickvars(e);
	l_ptr = e->first;
	t_ptr = (t_tetri*)l_ptr->content;
	while (l_ptr && e->letter <= 'Z')
	{
		t_ptr->letter = e->letter;
		e->tcount++;
		fillit_xy_get(t_ptr);
		fillit_xy_correct(t_ptr);
		fillit_order_get(t_ptr);
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
	if (e->letter == 'Z')
		fillit_error("too many tetriminos, 25 max.", DEBUG_MODE);
}
