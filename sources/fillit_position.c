/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/06 21:20:05 by lpoujade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function checks if the currently moving tetri is colliding with any
** tetri which position has been set. In case of a collision, 1 is returned.
*/

int	fillit_check_collision(t_env *e, t_tetri *moving)
{
	t_list	*lst_ptr;
	t_tetri	*fixed;

	lst_ptr = e->first;
	fixed = lst_ptr->content;
	while (lst_ptr)
	{
		fillit_reset_quickvars(e);
		while (fixed->set && e->i < 4)
		{
			e->j = 0;
			if (fillit_xy_collision(fixed->x[e->i] + fixed->x_offset,
						fixed->y[e->i] + fixed->y_offset, moving))
				return (1);
			e->i++;
		}
		lst_ptr = lst_ptr->next;
		fixed = lst_ptr->content;
	}
	return (0);
}

/*
** This function checks if the currently moving tetri is in contact with any
** tetri which position has been set. In case of contact, 1 is returned.
*/

int	fillit_check_contact(t_env *e, t_tetri *moving)
{
	fixed = lst_ptr->content;
	while (lst_ptr)
	{
		fillit_reset_quickvars(e);
		while (fixed->set && e->i < 4)
		{
			e->x = fixed->x[e->i] + fixed->x_offset;
			e->y = fixed->y[e->i] + fixed->y_offset;
			if (fillit_xy_collision(e->x + 1, e->y, moving) || 
					fillit_xy_collision(e->x - 1, e->y, moving) ||
					fillit_xy_collision(e->x, e->y + 1, moving) ||
					fillit_xy_collision(e->x, e->y - 1, moving))
				return (1);
			e->i++;
		}
		lst_ptr = lst_ptr->next;
		fixed = lst_ptr->content;
	}
	return (0);
}

/*
** Function to calc size of the square containing all fixed tretriminos plus
** the one which is moving.
** Calc is done simply by : (max x) * (max y)
*/

int fillit_square_size(t_env *e, t_tetri *moving)
{
	int		m_x;
	int		m_y;

	t_list	*lst_ptr;
	t_tetri	*fixed;

	lst_ptr = e->first;
	fixed = (t_tetri *)lst_ptr->content;
	m_x = moving->x_offset;
	m_y = moving->y_offset;
	while (lst_ptr)
	{
		if (fixed->set)
		{
			m_x += fixed->x_offset;
			m_y += fixed->y_offset;
		}
		if ((lst_ptr = lst_ptr->next))
			fixed = (t_tetri *)lst_ptr->content;
	}
	return (m_x * m_y);
}

/*
** This function returns 1 if the point at coordinates x,y collides with any
** block within the tetri represented by ptr.
*/

int	fillit_xy_collision(int x, int y, t_tetri *ptr)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (ptr->x[i] + ptr->x_offset == x && ptr->y[i] + ptr->y_offset == y)
			return (1);
		i++;
	}
	return (0);
}
