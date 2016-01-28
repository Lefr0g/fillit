/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/01/28 17:04:38 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

void	debug_print_t_tetri(t_tetri *t)
{
	fillit_print_single_tetri(t);
	fillit_print_xy(t);
	ft_putstr("fixed = ");
	ft_putnbr(t->fixed);
	ft_putstr("\n");
}

/*
** 0/ Se mettre en debut de liste
**   1/ Positionner le tetri
**   2/ Immobiliser le tetri
**   3/ Lancer recursivement jusqu'a ce que tous les tetris soient fixes
** 4/ Deverrouiller le tetri
** 5/ Selectionner le tetri suivant, retour en 1/ jusqu'au dernier tetri
*/

void	fillit_solve(t_env *e)
{
	t_vars	v;

	e->inception++;
	fillit_init_vars(&v);
	v.lst_ptr = e->first;
	while (v.lst_ptr)
	{
		v.tet_ptr = (v.lst_ptr)->content;
		if (!v.tet_ptr->fixed)
		{
			v.tet_ptr->fixed = 1;

// 			do stuff
			fillit_solve(e);

//			clear_screen
			ft_putstr("\033[2J");

			ft_putchar('\n');
			debug_inception_print(e);
			ft_putstr("\033[35mTetri ");
			ft_putchar(v.tet_ptr->letter);
			ft_putstr("\033[0m :\n");
			ft_print_memory(v.tet_ptr, sizeof(t_tetri));
			debug_print_t_tetri(v.tet_ptr);
			usleep(100000);

			v.tet_ptr->fixed = 0;
		}
		v.lst_ptr = v.lst_ptr->next;
	}
	e->inception--;
}
