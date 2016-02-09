/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/09 17:50:08 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function gets the values of the most extremes coordinates of the fixed
** tetriminos assembly.
*/

void	fillit_get_fixed_range(t_env *e, t_vars *v)
{
	int		t;
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	lst_ptr = e->first;
//	printf("Entering get_fixed_range\n");
	while (lst_ptr)
	{
		tet_ptr = lst_ptr->content;
		if (tet_ptr->fixed)
		{
			if ((t = ft_tabmax(tet_ptr->x, 4) + tet_ptr->x_offset) > v->xmax)
				v->xmax = t;
			if ((t = ft_tabmax(tet_ptr->y, 4) + tet_ptr->y_offset) > v->ymax)
				v->ymax = t;
			if ((t = ft_tabmin(tet_ptr->x, 4) + tet_ptr->x_offset) < v->xmin)
				v->xmin = t;
			if ((t = ft_tabmin(tet_ptr->y, 4) + tet_ptr->y_offset) < v->ymin)
				v->ymin = t;
		}
		lst_ptr = lst_ptr->next;
	}
}

/*
** This function calculates and stores the width and height of all tetris
** within the list
*/
void	fillit_get_width_height_all(t_list *first)
{
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	lst_ptr = first;
	while (lst_ptr)
	{
		tet_ptr = (t_tetri*)lst_ptr->content;
		tet_ptr->x_max = ft_tabmax(tet_ptr->x, 4);
		tet_ptr->y_max = ft_tabmax(tet_ptr->y, 4);
		lst_ptr = lst_ptr->next;
	}
}

/*
** Inversion de la logique de resolution :
** 0/ Choisir une taille de carre (en partant du plus grand cote parmi tous
**    les tetris)
** 1/ Essayer toutes les combinaisions recursivement
**    1.0/ Si tous les tetris rentrent : sauver le resultat
**    1.1/ Sinon, augmenter le carre de 1 et retour en 1/
*/

int 	fillit_solve(t_env *e, t_list *moving)
{
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	if (!moving && e->tcount == e->tlocked)
	{
		e->result = fillit_get_output_map(e);
		return (1);
	}
	lst_ptr = moving;
	tet_ptr = (t_tetri*)moving->content;

	printf("Entering fillit_solve(), tetri %c\n", tet_ptr->letter);
	
	tet_ptr->x_offset = 0;
	tet_ptr->y_offset = 0;
	
	if (e->tcount == e->tlocked)
	{
		printf("fillit_solve(), tetri %c, SOLUTION FOUND\n", tet_ptr->letter);
		return (1);
	}

	while (tet_ptr->y_offset + tet_ptr->y_max < e->square_size)
	{
		while (tet_ptr->x_offset + tet_ptr->x_max < e->square_size)
		{
//			printf("solve() : pre-collision check...");
			if (!fillit_check_collision(e, tet_ptr))
			{
				printf("solve() : no collision, locking tetri %c\n", tet_ptr->letter);
				tet_ptr->fixed = 1;
				e->tlocked++;
				printf("%lu / %lu locked\n", e->tlocked, e->tcount);
				if (fillit_solve(e, moving->next))
					return (1);
				printf("Unlocking %c\n", tet_ptr->letter);
				tet_ptr->fixed = 0;
				e->tlocked--;
			}
			else
				printf("COLLISION\n");
			tet_ptr->x_offset++;

			printf("%c x_offset = %d\n", tet_ptr->letter, tet_ptr->x_offset);
		}
		tet_ptr->x_offset = 0;
		tet_ptr->y_offset++;
		printf("%c y_offset = %d\n", tet_ptr->letter, tet_ptr->y_offset);
	}
	tet_ptr->x_offset = 0;
	tet_ptr->y_offset = 0;
	if (tet_ptr->fixed)
	{
		tet_ptr->fixed = 0;
		e->tlocked--;
	}
//	printf("Exiting fillit_solve(), tetri %c, returning 0\n", tet_ptr->letter);
	return (0);
}
