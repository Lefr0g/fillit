/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/10 16:23:31 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int		fillit_browse_square(t_env *e, t_list *moving, t_tetri *tet_ptr)
{
	while (tet_ptr->y_offset + tet_ptr->y_max < e->square_size)
	{
		while (tet_ptr->x_offset + tet_ptr->x_max < e->square_size)
		{
			if (!fillit_check_collision(e, tet_ptr))
			{
				tet_ptr->fixed = 1;
				e->tlocked++;
				if (fillit_solve(e, moving->next))
					return (1);
				tet_ptr->fixed = 0;
				e->tlocked--;
			}
			tet_ptr->x_offset++;
		}
		tet_ptr->x_offset = 0;
		tet_ptr->y_offset++;
	}
	return (0);
}

/*
** This recursive function uses backtracking to find the best tetri assembly
*/

int		fillit_solve(t_env *e, t_list *moving)
{
	t_tetri	*tet_ptr;

	if (!moving && e->tcount == e->tlocked)
	{
		e->result = fillit_get_output_map(e);
		return (1);
	}
	tet_ptr = (t_tetri*)moving->content;
	tet_ptr->x_offset = 0;
	tet_ptr->y_offset = 0;
	if (fillit_browse_square(e, moving, tet_ptr))
		return (1);
	return (0);
}
