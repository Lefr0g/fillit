/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/01/28 18:49:11 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

void	debug_print_t_tetri(t_env *e, t_tetri *t)
{
	int	i;

//	clear_screen
	ft_putstr("\033[2J");

	ft_putchar('\n');
	debug_inception_print(e);
	ft_putstr("\033[35mTetri ");
	ft_putchar(t->letter);
	ft_putstr("\033[0m :\n");
	ft_print_memory(t, sizeof(t_tetri));
	i = ft_tabmax(t->y, 4);
	while (i < 4)
	{
		ft_putchar('\n');
		i++;
	}
	fillit_print_single_tetri(t);
	fillit_print_xy(t);
	ft_putstr("fixed = ");
	ft_putnbr(t->fixed);
	ft_putstr("\n");
	ft_putstr("firstmove = ");
	ft_putnbr(t->firstmove);
	ft_putstr("\n");
	usleep(100000);

}

/*
** This function displaces the moving tetri in a particular order around the
** group of fixed tetris.
** Each call to this function must move the tetri to the next position, until
** all positions have been tried.
*/
/*
void	fillit_set_position(t_env *e, t_tetri *ptr)
{
	0/ Trouver les coordonnees du bloc immobile en haut a droite (xmax, ymin),
	positionner le moving par rapport a ce point, le plus pres possible
		- Si collision ==> deplacer a droite
		- Si non-contact ==> deplacer a gauche
	1/ Au prochain appel, descendre de 1 (y++)
	    - Memes checks que pour 0/
		- Si ymin du moving == ymax des fixed, on passe au 2/

	2/ Positionner le moving par rapport au point fixe le plus en bas a gauche
	(xmin, ymax), positionner le moving par rapport a ce point le plus pres
	possible
		- Si collision ==> deplacer vers le bas
		- Si non-contact ==> deplacer vers le haut
	3/ Au prochain appel, deplacer vers la droite (x++)
		- Memes checks que pour 2/
		- Si xmin du moving == xmax des fixed, on passe au 4/

	4/ Ainsi de suite

HINTS : utilisation de pointeurs sur fonction.
}
*/

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
	if (e->tlocked == e->tcount)
		ft_putstr("\033[31mSNAP!\033[0m\n");
		// take snapshot
	else
	{
		while (v.lst_ptr)
		{
			v.tet_ptr = (v.lst_ptr)->content;
			if (!v.tet_ptr->fixed)
			{
//				fillit_set_position(e, v.tet_ptr);
				v.tet_ptr->fixed = 1;
				e->tlocked++;
				fillit_solve(e);

				// 			do stuff
				debug_print_t_tetri(e, v.tet_ptr);

				v.tet_ptr->fixed = 0;
				e->tlocked--;
			}
			v.lst_ptr = v.lst_ptr->next;
		}
	}
	e->inception--;
}
