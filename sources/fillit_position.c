/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/07 19:34:50 by amulin           ###   ########.fr       */
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

int	fillit_check_contact(t_env *e, t_tetri *moving)
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
		if (fixed->fixed)
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

/*
** Contrary to the functions above, this function takes the fixed tetris for
** reference. For each fixed block, it runs a test on its 4 neighbor coordinates.
** 
** This test consists in moving the tested tetri so that the neighbor coord
** matches with one of the tested tetri block.
** This test also ensures that in this position, the other blocks of the tested
** tetri are not colliding with any of the fixed blocks.
** If no collision is detected, the tested tetri is set as 'fixed', then we
** recursively lauch fillit_move_around() on the next moving tetri.
** 
** Once all tetris are fixed, we calculate the size of the smallest square
** containing all the fixed tetris plus the moving tetri in its current position.
** If this square is the smallest ever obtained, we take a "sreenshot" of the
** resulting grid, which will either be replaced by an upcoming run of this test
** or be printed on stdout at the end of the program if no smaller square could
** be obtained by any other positioning of the tetris.
*/

void	fillit_move_around(t_env *e)
{
	t_list	*lst_ptr_fixed;
	t_list	*lst_ptr_moving;
	t_tetri	*fixed;
	t_tetri	*moving;
	int		x_ref;
	int		y_ref;
	int		i;

	lst_ptr_moving = e->first;
	while (lst_ptr_moving)
	{
		moving = (t_tetri*)lst_ptr_moving->content;
		if (!moving->fixed)
		{
			lst_ptr_fixed = e->first;
			while (lst_ptr_fixed)
			{
				fixed = (t_tetri*)lst_ptr_fixed->content;
				i = 0;
				while (fixed->fixed && i < 4)
				{
					x_ref = fixed->x[i] + fixed->x_offset;
					y_ref = fixed->y[i] + fixed->y_offset;
					fillit_move_and_try(e, moving, x_ref + 1, y_ref);
					fillit_move_and_try(e, moving, x_ref - 1, y_ref);
					fillit_move_and_try(e, moving, x_ref, y_ref + 1);
					fillit_move_and_try(e, moving, x_ref, y_ref - 1);
					e->i++;
					if ((lst_ptr_fixed = lst_ptr_fixed->next))
						fixed = lst_ptr_fixed->content;
				}
				lst_ptr_fixed = lst_ptr_fixed->next;
			}
		}
		lst_ptr_moving = lst_ptr_moving->next;
	}
}

/*
** The following routine displaces the moving tetrimino to center each of its
** blocks on the coordinates x,y.
** If no collision occurs with other fixed tetriminos and if all tetriminos are
** locked in position, then we calculate the size of the containing square.
** If this square is the smallest ever calculated, a 'screenshot' of the grid
** is saved.
*/

void	fillit_move_and_try(t_env *e, t_tetri *moving, int x, int y)
{
	int		j;
	j = 0;
	while (j < 4)
	{
		moving->x_offset = x + moving->x[j];
		moving->y_offset = y + moving->y[j];
		if (!(fillit_check_collision(e, moving)))
		{
			moving->fixed = 1;
			e->tlocked++;
			if (e->tlocked == e->tcount)
			{
				//	calculer carre;
//				if (taille_du_carre < e->smallest_size)
					//	screenshot;
			}
			else
				fillit_move_around(e);
			moving->fixed = 0;
			e->tlocked--;
		}
		j++;
	}
}
