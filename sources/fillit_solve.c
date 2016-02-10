/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/10 15:02:34 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

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

	tet_ptr->x_offset = 0;
	tet_ptr->y_offset = 0;
	
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
	tet_ptr->x_offset = 0;
	tet_ptr->y_offset = 0;
	return (0);
}
