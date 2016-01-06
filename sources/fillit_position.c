/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_position.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/06 15:36:18 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function checks if the currently moving tetri is colliding with any
** tetri which position has been set. In case of a collision, 1 is returned.
*/

int	fillit_check_position(t_env *e, t_tetri *moving)
{
	t_list	*lst_ptr;
	t_tetri	*fixed;

	lst_ptr = e->first;
	fixed = list_ptr->content;
	while (lst_ptr)
	{
		fillit_reset_quickvars(e);
		if (fixed->set)
		{
			// Do the check
			while (i < 4)
			{
				if (moving->x[i] + moving->x_offset == fixed->x[i] + 
						fixed->x_offset && moving->y[i] + moving->y_offset ==
						fixed->y[i] + fixed->y_offset)
					return (1);
				else
					i++;
			}
		}
		lst_ptr = lst_ptr->next;
		fixed = lst_ptr->content;
	}
	return (0);
}
