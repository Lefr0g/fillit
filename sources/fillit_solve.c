/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/01/28 14:41:43 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

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
//			if (!t->tet_mov->firstmove)
				fillit_move_and_try(e, t->tet_mov, t->xref, t->yref - 1);
			t->tet_mov->firstmove = 0;
		}
		t->i++;
	}
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
//
	char	*map;

	if (!e->smallest_size)
		e->smallest_size = 10;
	map = ft_strnew(e->smallest_size * (e->smallest_size + 1) + 1);
//
	siz_square = 0;
	j = 0;
	while (j < 4 && (!e->smallest_size ||
				(fillit_square_size(e) < e->smallest_size &&
			e->smallest_size * e->smallest_size >= ((int)e->tcount * 4) +
			(e->smallest_size * 2 - 1))) && x >= 0 && y >= 0)
	{
		moving->x_offset = x - moving->x[j];
		moving->y_offset = y - moving->y[j];
//		moving->x_offset = x - moving->x[moving->order[3 - j]];
//		moving->y_offset = y - moving->y[moving->order[3 - j]];

//		fillit_print_raw(e);

		if (!(fillit_check_collision(e, moving)))
		{
			moving->fixed = 1;
			e->tlocked++;
//			debug_inception_print(e);
//			printf("Tetri %c locked. %lu on %lu are locked\n", moving->letter, e->tlocked, e->tcount);
			if (e->tlocked == e->tcount)
			{
//
//				if (DEBUG_MODE)
//				{
//					fillit_print_raw(e);
//					fillit_save_printable(e, &map);
//					ft_putendl(map); ft_putchar('\n');
//				}
//
				fillit_check_solution(e, &siz_square);
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

void	fillit_check_solution(t_env *e, int *siz_square)
{		
//	debug_inception_print(e);
//	printf("\033[36mAll tetris are locked\n\033[0m");
	*siz_square = fillit_square_size(e);
	if ((!e->smallest_size || *siz_square < e->smallest_size))
	{
		if (DEBUG_MODE)
		{
			debug_inception_print(e);
			printf("\033[31mCurrent square is the smallest! \033[0m(%d)\n",
				*siz_square);
		}
		e->smallest_size = *siz_square;
		if (!e->result)
			e->result = (char *)ft_memalloc(e->smallest_size
					* (e->smallest_size + 1 ) + 1);
		fillit_save_printable(e, &e->result);
	}
	if (e->update)
		fillit_liveprint(e);
}


