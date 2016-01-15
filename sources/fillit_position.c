/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/15 17:43:46 by amulin           ###   ########.fr       */
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
** Function to calc size of the square containing all fixed tretriminos plus the one moving
** OR
** if no moving piece : calc the size of the map with all tetriminos
*/

int fillit_square_size(t_env *e)
{
	int		xmax;
	int		ymax;
	int		xmin;
	int		ymin;
	int		t;

	xmax = INT_MIN;
	ymax = INT_MIN;
	xmin = INT_MAX;
	ymin = INT_MAX;
	t_list	*lst_ptr;
	t_tetri	*fixed;
	lst_ptr = e->first;
	fixed = (t_tetri *)lst_ptr->content;
	while (lst_ptr)
	{
		if (fixed->fixed)
		{
			if ((t = fixed->x_offset + ft_tabmax(fixed->x, 4)) > xmax)
				xmax = t;
			if ((t = fixed->y_offset + ft_tabmax(fixed->y, 4)) > ymax)
				ymax = t;
			if ((t = fixed->x_offset + ft_tabmin(fixed->x, 4)) < xmin)
				xmin = t;
			if ((t = fixed->y_offset + ft_tabmin(fixed->y, 4)) < ymin)
				ymin = t;
		}
		if ((lst_ptr = lst_ptr->next))
			fixed = (t_tetri *)lst_ptr->content;
	}
	e->xmin = xmin;
	e->ymin = ymin;
	if (xmax - xmin > ymax - ymin)
		return (xmax - xmin + 1);
	else
		return (ymax - ymin + 1);
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
** If no collision is detected, the tested tetri is set as 'fixed'.
** 
** OPTIMZATION :
** Then, we calculate the size of the square containing all fixed tetris.
** 
** If this square is smaller than the smallest 'fully filled' square, we 
** recursively launch fillit_move_around() on the next moving tetri.
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

	e->inception++;
//	lst_ptr_moving = e->first;
	lst_ptr_moving = e->last;

//	debug_inception_print(e);
//	printf("\033[35mENTERING fillit_move_around()\033[0m\n");
	while (lst_ptr_moving)
	{
		moving = (t_tetri*)lst_ptr_moving->content;
		if (!moving->fixed)
		{
//			debug_inception_print(e);
//			printf("Moving tetri %c\n", moving->letter);

//			lst_ptr_fixed = e->first;
			lst_ptr_fixed = e->last;
			while (lst_ptr_fixed)
			{
				fixed = (t_tetri*)lst_ptr_fixed->content;
				i = 0;
				while (fixed->fixed && i < 4)
				{
					if (!e->smallest_size || fillit_square_size(e)
							< e->smallest_size)
					{
//						debug_inception_print(e);
//						printf("\033[32mLaunching move_and_try() on moving tetri %c, i = %d\033[0m\n", moving->letter, i);
						x_ref = fixed->x[i] + fixed->x_offset;
						y_ref = fixed->y[i] + fixed->y_offset;
						fillit_move_and_try(e, moving, x_ref + 1, y_ref);
						fillit_move_and_try(e, moving, x_ref - 1, y_ref);
						fillit_move_and_try(e, moving, x_ref, y_ref + 1);
						fillit_move_and_try(e, moving, x_ref, y_ref - 1);
					}
					i++;
//					ft_putchar('-');
				}
//				if ((lst_ptr_fixed = lst_ptr_fixed->next))
				if ((lst_ptr_fixed = lst_ptr_fixed->prev))
					fixed = lst_ptr_fixed->content;
			}
		}
//		lst_ptr_moving = lst_ptr_moving->next;
		lst_ptr_moving = lst_ptr_moving->prev;
	}
//	debug_inception_print(e);
//	printf("\033[35mLEAVING fillit_move_around()\033[0m\n");
	e->inception--;
}

/*
** The following routine displaces the moving tetrimino to center each of its
** blocks on the coordinates x,y.
** If no collision occurs with other fixed tetriminos and if all tetriminos are
** locked in position, then we calculate the size of the containing square.
** Once all tetris are fixed, we calculate the size of the smallest square
** containing all the fixed tetris.
** If this square is the smallest ever obtained, we take a "sreenshot" of the
** resulting grid, which will either be replaced by an upcoming run of this test
** or be printed on stdout at the end of the program if no smaller square could
** be obtained by any other positioning of the tetris.
*/

void	fillit_move_and_try(t_env *e, t_tetri *moving, int x, int y)
{
	int		j;
	int		siz_square;

	siz_square = 0;
	j = 0;
	while (j < 4 && (!e->smallest_size || fillit_square_size(e) < e->smallest_size))
	{
		moving->x_offset = x - moving->x[j];
		moving->y_offset = y - moving->y[j];
//		ft_putendl(fillit_save_printable(e));
		if (!(fillit_check_collision(e, moving)))
		{
			moving->fixed = 1;
			e->tlocked++;
//			debug_inception_print(e);
//			printf("Tetri %c locked. %lu on %lu are locked\n", moving->letter, e->tlocked, e->tcount);
			if (e->tlocked == e->tcount)
			{
//				debug_inception_print(e);
//				printf("\033[36mAll tetris are locked\n\033[0m");
				siz_square = fillit_square_size(e);
				if (!e->smallest_size || siz_square < e->smallest_size)
				{
					debug_inception_print(e);
					printf("\033[31mCurrent square is the smallest! \033[0m(%d)\n",
							siz_square);
					e->smallest_size = siz_square;
					e->result = fillit_save_printable(e);
					ft_putendl(e->result);
				}
//				else
//					ft_putendl(fillit_save_printable(e));
//				ft_putchar('|');
			}
			else
			{
				fillit_move_around(e);
			}
			moving->fixed = 0;
//			debug_inception_print(e);
//			printf("Tetri %c unlocked\n", moving->letter);
			e->tlocked--;
		}
		j++;
		if (e->smallest_size == 6 && e->tlocked > 5)
			printf("%lu / %lu locked\n", e->tlocked, e->tcount);
	}
}

void	debug_inception_print(t_env *e)
{
	int	i;

	i = 0;
	while (i < e->inception)
	{
		ft_putstr("\033[33m> \033[0m");
		i++;
	}
}
