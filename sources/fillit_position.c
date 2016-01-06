/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/06 20:57:45 by lpoujade         ###   ########.fr       */
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
		if (fixed->set)
		{
			// Do the check
			while (e->i < 4)
			{
				e->j = 0;
				while (e->j < 4)
				{
					if (moving->x[e->i] + moving->x_offset == fixed->x[e->i] + 
							fixed->x_offset && moving->y[e->j] +
							moving->y_offset == fixed->y[e->j] + fixed->y_offset)
						return (1);
					else
						e->j++;
				}
				e->i++;
			}
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
	m_x = ft_tabmax(moving->x, 4);
	m_y = ft_tabmax(moving->y, 4);
	while (lst_ptr)
	{
		if (fixed->set)
		{
			if (ft_tabmax(fixed->x, 4) > m_x)
				m_x = ft_tabmax(fixed->x, 4);
			if (ft_tabmax(fixed->y, 4) > m_y)
				m_y = ft_tabmax(fixed->y, 4);
		}
		if ((lst_ptr = lst_ptr->next))
			fixed = (t_tetri *)lst_ptr->content;
	}
	return (m_x * m_y);

