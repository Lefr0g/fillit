/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/02/12 17:45:50 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function calculates and stores the width and height of all tetris
** within the list. The results are saved in each t_tetri as x_max and y_max
*/

void	fillit_get_width_height_all(t_list *first)
{
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	lst_ptr = first;
	while (lst_ptr)
	{
		tet_ptr = (t_tetri*)lst_ptr->content;
		tet_ptr->x_max = ft_tabmax(tet_ptr->x, 4);
		tet_ptr->y_max = ft_tabmax(tet_ptr->y, 4);
		lst_ptr = lst_ptr->next;
	}
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
	char	letter;

	letter = 'A';
	e->tcount = 0;
	e->tlocked = 0;
	fillit_reset_quickvars(e);
	l_ptr = e->first;
	t_ptr = (t_tetri*)l_ptr->content;
	while (l_ptr && letter <= 'Z')
	{
		t_ptr->letter = letter;
		e->tcount++;
		fillit_xy_get(t_ptr);
		fillit_xy_correct(t_ptr);
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		letter++;
		fillit_reset_quickvars(e);
	}
	if (letter == 'Z')
		fillit_error("too many tetriminos, 26 max.", DEBUG_MODE);
	fillit_get_width_height_all(e->first);
}

/*
** This function gets the values of the most extremes coordinates of the fixed
** tetriminos assembly.
*/

void	fillit_get_fixed_range(t_env *e, t_vars *v)
{
	int		t;
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	lst_ptr = e->first;
	while (lst_ptr)
	{
		tet_ptr = lst_ptr->content;
		if (tet_ptr->fixed)
		{
			if ((t = ft_tabmax(tet_ptr->x, 4) + tet_ptr->x_offset) > v->xmax)
				v->xmax = t;
			if ((t = ft_tabmax(tet_ptr->y, 4) + tet_ptr->y_offset) > v->ymax)
				v->ymax = t;
			if ((t = ft_tabmin(tet_ptr->x, 4) + tet_ptr->x_offset) < v->xmin)
				v->xmin = t;
			if ((t = ft_tabmin(tet_ptr->y, 4) + tet_ptr->y_offset) < v->ymin)
				v->ymin = t;
		}
		lst_ptr = lst_ptr->next;
	}
}
