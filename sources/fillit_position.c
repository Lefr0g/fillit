/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/02/10 14:53:20 by amulin           ###   ########.fr       */
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
		while (fixed->fixed && e->i < 4)
		{
//			e->j = 0;
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

int     fillit_check_contact(t_env *e, t_tetri *moving)
{
	t_list  *lst_ptr;
	t_tetri *fixed;

	lst_ptr = e->first;
	fixed = lst_ptr->content;
	while (lst_ptr)
	{
		fillit_reset_quickvars(e);
		while (fixed->fixed && e->i < 4)
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

void	fillit_liveprint(t_env *e)
{
	ft_putstr("\e[1;1H\e[2J");
	ft_putendl(" ========== Status ========== ");
	ft_putstr("\033[33m Current square\t:\033[0m\t");
	ft_putnbr(e->smallest_size);
	ft_putchar('\n');
	ft_putstr("\033[33m Imbrication\t:\033[0m\t");
	ft_putnbr(e->inception);
	ft_putchar('\n');ft_putchar('\n');
	//					if (e->color)
	//						fillit_print_colored(fillit_save_printable(e));
	//					else
	//						ft_putendl(fillit_save_printable(e));
}
