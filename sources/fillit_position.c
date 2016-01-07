/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/07 17:54:42 by lpoujade         ###   ########.fr       */
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
		if ((lst_ptr = lst_ptr->next))
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
	t_list	*lst_ptr;
	t_tetri	*fixed;

	lst_ptr = e->first;
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
		if ((lst_ptr = lst_ptr->next))
			fixed = lst_ptr->content;
	}
	return (0);
}

/*
** Function to calc size of the square containing all fixed tretriminos plus
** the one which is moving ( if there is one, else only those which are fixed )
*/

int fillit_square_size(t_env *e, t_tetri *moving)
{
	int		c;
	int		t;

	t_list	*lst_ptr;
	t_tetri	*fixed;
	c = 0;
	lst_ptr = e->first;
	fixed = (t_tetri *)lst_ptr->content;
	if (moving)
		if ((c = moving->x_offset + 1 + ft_tabmax(moving->x, 4)) < (t = moving->y_offset + 1 + ft_tabmax (moving->y, 4)))
			c = t;
	while (lst_ptr)
	{
		if (fixed->set)
		{
			ft_putstr("Fixed : ");ft_putendl(&fixed->letter);
			if ((t = fixed->x_offset + 1 + ft_tabmax(fixed->x, 4)) > c)
				c = t;
			t = fixed->y_offset + 1 + ft_tabmax(fixed->y, 4);
			if ((t = fixed->y_offset + 1 + ft_tabmax(fixed->y, 4)) > c)
				c = t;
		}
		if ((lst_ptr = lst_ptr->next))
			fixed = (t_tetri *)lst_ptr->content;
	}
	return (c);
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
