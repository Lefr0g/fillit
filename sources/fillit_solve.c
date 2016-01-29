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
** This function gets the values of the most extremes coordinates of the fixed
** tetriminos assembly.
*/

void	fillit_get_fixed_range(t_env *e, t_vars *v)
{
	int	t;

	v->lst_ptr = e->first;
	while (v->lst_ptr)
	{
		v->tet_ptr = v->lst_ptr->content;
		if (v->tet_ptr->fixed)
		{
			if ((t = ft_tabmax(v->tet_ptr->x, 4) + v->tet_ptr->x_offset) > v->xmax)
				v->xmax = t;
			if ((t = ft_tabmax(v->tet_ptr->y, 4) + v->tet_ptr->y_offset) > v->ymax)
				v->ymax = t;
			if ((t = ft_tabmin(v->tet_ptr->x, 4) + v->tet_ptr->x_offset) < v->xmin)
				v->xmin = t;
			if ((t = ft_tabmin(v->tet_ptr->y, 4) + v->tet_ptr->y_offset) < v->ymin)
				v->ymin = t;
		}
		v->lst_ptr = v->lst_ptr->next;
	}

}

/*
** This functions moves the free tetrimino down along the right side of the
** fixed tetris assembly.
** It ensures that the moving tetri is in contact but not colliding with
** any blocks of the fixed tetri assembly.
*/

void	fillit_move_along_right(t_env *e, t_vars *v, t_tetri *moving)
{
	printf("fillit_move_along_right START\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmax;
		moving->y_offset = v->ymin;
		printf("moving->x_offset = %d, moving->y_offset = %d\n",
				moving->x_offset, moving->y_offset);
		v->move_engaged = 1;
	}
	else
		moving->y_offset += 1;
	printf("fillit_move_along_right MIDDLE\n");
	if (ft_tabmin(moving->y, 4) + moving->y_offset > v->ymax)
	{
		v->side++;
		v->move_engaged = 0;
	}
	else
	{
		printf("fillit_move_along_right pre-adjustment loop\n");
		while (fillit_check_collision(e, moving)
				|| !fillit_check_contact(e, moving))
		{
			printf("fillit_move_along_right adjustment loop\n");
			printf("Collision = %d, contact = %d\n",
					fillit_check_collision(e, moving),
					fillit_check_contact(e, moving));
			printf("moving->x_offset = %d, moving->y_offset = %d\n",
					moving->x_offset, moving->y_offset);
			if (fillit_check_collision(e, moving))
				moving->x_offset += 1;
			else if (!fillit_check_contact(e, moving))
				moving->x_offset -= 1;
			usleep(100000);
		}
	}
//	printf("fillit_move_along_right END\n");
}

void	fillit_move_along_bottom(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymax;
		v->move_engaged = 1;
	}
	else
		moving->x_offset += 1;
	if (ft_tabmin(moving->x, 4) + moving->x_offset > v->xmax)
	{
		v->side++;
		v->move_engaged = 0;
	}
	else
	{
		while (fillit_check_collision(e, moving)
				|| !fillit_check_contact(e, moving))
		{
			if (fillit_check_collision(e, moving))
				moving->y_offset += 1;
			else if (!fillit_check_contact(e, moving))
				moving->y_offset -= 1;
		}
	}
}

void	fillit_move_along_left(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->y_offset += 1;
	if (ft_tabmin(moving->y, 4) + moving->y_offset > v->ymax)
	{
		v->side++;
		v->move_engaged = 0;
	}
	else
	{
		while (fillit_check_collision(e, moving)
				|| !fillit_check_contact(e, moving))
		{
			if (fillit_check_collision(e, moving))
				moving->x_offset -= 1;
			else if (!fillit_check_contact(e, moving))
				moving->x_offset += 1;
		}
	}
}

void	fillit_move_along_top(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->x_offset += 1;
	if (ft_tabmin(moving->x, 4) + moving->x_offset > v->xmax)
	{
		v->side++;
		v->move_engaged = 0;
	}
	else
	{
		while (fillit_check_collision(e, moving)
				|| !fillit_check_contact(e, moving))
		{
			if (fillit_check_collision(e, moving))
				moving->y_offset -= 1;
			else if (!fillit_check_contact(e, moving))
				moving->y_offset += 1;
		}
	}
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

void	fillit_set_position(t_env *e, t_vars *v, t_tetri *moving)
{
	ft_putstr("Entering set_position\n");
	if (v->side == 0)
		fillit_move_along_right(e, v, moving);
	else if (v->side == 1)
		fillit_move_along_bottom(e, v, moving);
	else if (v->side == 2)
		fillit_move_along_left(e, v, moving);
	else if (v->side == 3)
		fillit_move_along_top(e, v, moving);
	ft_putstr("Leaving set_position\n");
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
	if (e->tlocked == e->tcount)
	{
//		fillit_save_printable(e, &e->result);
		e->smallest_size = fillit_square_size(e);
		printf("Smallest square is %d\n", e->smallest_size);
	}
	else
	{
		while (v.lst_ptr)
		{
			v.tet_ptr = (v.lst_ptr)->content;
			fillit_print_xy(v.tet_ptr);
			printf("Tetri %c fixed = %d\n", v.tet_ptr->letter, v.tet_ptr->fixed);
			if (!v.tet_ptr->fixed)
			{
				fillit_get_fixed_range(e, &v);
				printf("xmin = %d, xmax = %d\n", v.xmin, v.xmax);
				printf("ymin = %d, ymax = %d\n", v.ymin, v.ymax);
				while (v.side < 4)
				{
					printf("Check, side = %d\n", v.side);
					fillit_set_position(e, &v, v.tet_ptr);
					ft_putstr("set_position OK\n");
					v.tet_ptr->fixed = 1;
					e->tlocked++;
					fillit_solve(e);

					debug_print_t_tetri(e, v.tet_ptr);
					v.tet_ptr->fixed = 0;
					e->tlocked--;
				}

			}
			v.lst_ptr = v.lst_ptr->next;
		}
	}
	e->inception--;
}
