/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/02/10 14:50:18 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

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
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
	if (e->letter == 'Z')
		fillit_error("too many tetriminos, 25 max.", DEBUG_MODE);
}
