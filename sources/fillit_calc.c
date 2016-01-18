/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_calc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpoujade <lpoujade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 18:38:28 by lpoujade          #+#    #+#             */
/*   Updated: 2016/01/18 17:46:43 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function finds out the order of the blocks, so we can rotate
** around the tetri clockwise, starting top-left of fixed tetri, according to 
** the rules defined in the subject.
** The result will be stored in a int[4], within each t_tetri.
** This data will be used in the high-level loop of the fillit_move_around()
** function instead of the simple i increment.
*/

void	fillit_order_get(t_tetri *ptr)
{
	int	i;
	int	j;
	int	xmax;
//	int	xmin;
//	int	ymin;
	int	ymax;
	int	height;
	int	isaved;
	int	pos;
	int	multiflag;
	
//	i = 3;
//	j = 0;
//	xmax = ft_tabmax(ptr->x, 4);
//	ymin = 0;

	ymax = ft_tabmax(ptr->y, 4);
	xmax = -1;
	i = 0;
	isaved = -1;
	j = 0;
	height = 0;
	pos = 0;
	multiflag = 0;

	while (height <= ymax)
	{
		i = 0;
		xmax = INT_MIN;
		while (i < 4)
		{
			if (ptr->y[i] == height && ptr->x[i] > xmax)
			{
				xmax = ptr->x[i];
				isaved = i;
			}
			i++;
		}
		if (isaved != -1)
		{
			ptr->order[pos] = isaved;
//			printf("Tetri %c, ptr->order[%d] = %d (top-down)\n", ptr->letter, pos, isaved);
			pos++;
			isaved = -1;
		}
		height++;
	}
	while (height >= 0)
	{
		xmax = INT_MIN;
		i = 0;
		while (i < 4)
		{
			if (ptr->y[i] == height && ptr->x[i] > xmax)
				xmax = ptr->x[i];
			i++;
		}
		i = 0;
		while (i < 4)
		{
			if (ptr->y[i] == height && ptr->x[i] == xmax - 1)
			{
//				printf("Tetri %c, ptr->order[%d] = %d (bottom-up)\n", ptr->letter, pos, i);
				ptr->order[pos] = i;
				pos++;
				xmax--;
				i = -1;
			}
			i++;
		}
		height--;
	}
/*	
	while (j < 4)
	{
		if (ptr->y[i] == ymin)
		{
			ptr->order[j] = i;
			j++;
		}
		i--;
		if (i < 0)
		{
			i = 3;
			ymin++;
		}
	}
*/
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
		fillit_order_get(t_ptr);
		if ((l_ptr = l_ptr->next))
			t_ptr = (t_tetri*)l_ptr->content;
		e->letter++;
		fillit_reset_quickvars(e);
	}
	if (e->letter == 'Z')
		fillit_error("too many tetriminos, 25 max.");
}
