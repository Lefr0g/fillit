/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/26 14:53:29 by amulin           ###   ########.fr       */
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
** Sub-function for fillit_square_size(), cut for norme reasons.
** It changes the values of the most extreme blocks in the square if a block
** is found beyond these limits.
*/

void	fillit_square_get_range(t_vars *t)
{
	if ((t->i = t->tet_fix->x_offset + ft_tabmax(t->tet_fix->x, 4)) > t->xmax)
		t->xmax = t->i;
	if ((t->i = t->tet_fix->y_offset + ft_tabmax(t->tet_fix->y, 4)) > t->ymax)
		t->ymax = t->i;
	if ((t->i = t->tet_fix->x_offset + ft_tabmin(t->tet_fix->x, 4)) < t->xmin)
		t->xmin = t->i;
	if ((t->i = t->tet_fix->y_offset + ft_tabmin(t->tet_fix->y, 4)) < t->ymin)
		t->ymin = t->i;
}

/*
** Function to calc size of the square containing all fixed tretriminos
** plus the one moving
** OR
** if no moving piece : calc the size of the map with all tetriminos
*/

int fillit_square_size(t_env *e)
{
	t_vars	t;

	fillit_init_vars(&t);
	t.lst_ptr = e->first;
	t.tet_fix = (t_tetri *)t.lst_ptr->content;
	while (t.lst_ptr)
	{
		if (t.tet_fix->fixed)
		{
			fillit_square_get_range(&t);
		}
		if ((t.lst_ptr = t.lst_ptr->next))
			t.tet_fix = (t_tetri *)t.lst_ptr->content;
	}
	e->xmin = t.xmin;
	e->ymin = t.ymin;
	if (t.xmax - t.xmin > t.ymax - t.ymin)
		return (t.xmax - t.xmin + 1);
	else
		return (t.ymax - t.ymin + 1);
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
