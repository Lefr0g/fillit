/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_position.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 15:15:19 by amulin            #+#    #+#             */
/*   Updated: 2016/01/26 12:48:03 by amulin           ###   ########.fr       */
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

/*
** This function tries to position the moving tetri in any possible way around
** one block of the fixed tetris assembly.
*/
void	fillit_move_around(t_env *e, t_vars *t)
{ 
	t->i = 0;
	while (t->tet_fix->fixed && t->i < 4)
	{
		if ((!e->smallest_size || fillit_square_size(e)
					< e->smallest_size))
		{
//			debug_inception_print(e);
//			printf("\033[32mLaunching move_and_try() on moving tetri %c, i = %d\033[0m\n", moving->letter, i);
			t->xref = t->tet_fix->x[t->tet_fix->order[t->i]] + t->tet_fix->x_offset;
			t->yref = t->tet_fix->y[t->tet_fix->order[t->i]] + t->tet_fix->y_offset;

			fillit_move_and_try(e, t->tet_mov, t->xref + 1, t->yref);
			fillit_move_and_try(e, t->tet_mov, t->xref, t->yref + 1);
			fillit_move_and_try(e, t->tet_mov, t->xref - 1, t->yref);
			if (!t->tet_mov->firstmove)
				fillit_move_and_try(e, t->tet_mov, t->xref, t->yref - 1);
			t->tet_mov->firstmove = 0;
		}
		t->i++;
	}
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
** recursively launch fillit_solve() on the next moving tetri
** via fillit_move_around().
*/

void	fillit_solve(t_env *e)
{
	t_vars	t;

	e->inception++;
	t.lst_mov = e->first;
//	debug_inception_print(e);
//	printf("\033[35mENTERING fillit_solve()\033[0m\n");
	while (t.lst_mov)
	{
		t.tet_mov = (t_tetri*)t.lst_mov->content;
		if (!t.tet_mov->fixed)
		{
//			debug_inception_print(e);
//			printf("Moving tetri %c\n", moving->letter);
			t.lst_fix = e->first;
			t.tet_fix = (t_tetri*)t.lst_fix->content;
			while (t.lst_fix)
			{
				fillit_move_around(e, &t);
				if ((t.lst_fix = t.lst_fix->next))
					t.tet_fix = t.lst_fix->content;
			}
		}
		t.lst_mov = t.lst_mov->next;
	}
//	debug_inception_print(e);
//	printf("\033[35mLEAVING fillit_solve()\033[0m\n");
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
	while (j < 4 && (!e->smallest_size ||
				(fillit_square_size(e) < e->smallest_size &&
			e->smallest_size * e->smallest_size >= ((int)e->tcount * 4) +
			(e->smallest_size * 2 - 1))) && x >= 0 && y >= 0)
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
				if ((!e->smallest_size || siz_square < e->smallest_size))
				{
					debug_inception_print(e);
					printf("\033[31mCurrent square is the smallest! \033[0m(%d)\n",
							siz_square);
					e->smallest_size = siz_square;
					if (!e->result)
						e->result = (char *)ft_memalloc(e->smallest_size
								* (e->smallest_size + 1 ) + 1);
					fillit_save_printable(e, &e->result);

				}
				if (e->update)
					fillit_liveprint(e);
			}
			else
				fillit_solve(e);
			moving->fixed = 0;
			moving->firstmove = 1;
//			debug_inception_print(e);
//			printf("Tetri %c unlocked\n", moving->letter);
			e->tlocked--;
		}
		j++;
//		if (e->smallest_size == 6 && e->tlocked > 5)
//			printf("%lu / %lu locked\n", e->tlocked, e->tcount);
	}
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
