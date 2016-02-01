/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/01 16:41:53 by amulin           ###   ########.fr       */
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
	int		t;
	t_list	*lst_ptr;
	t_tetri	*tet_ptr;

	lst_ptr = e->first;
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
** This functions moves the free tetrimino down along the right side of the
** fixed tetris assembly.
** It ensures that the moving tetri is in contact but not colliding with
** any blocks of the fixed tetri assembly.
*/

void	fillit_move_along_right(t_env *e, t_vars *v, t_tetri *moving)
{
//	printf("fillit_move_along_right START\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmax;
		moving->y_offset = v->ymin;
//		printf("Positionning %c at top-left :\n", moving->letter);
//		printf("\tmoving->x_offset = %d, moving->y_offset = %d\n",
//				moving->x_offset, moving->y_offset);
		v->move_engaged = 1;
	}
	else
		moving->y_offset += 1;
//	printf("fillit_move_along_right MIDDLE\n");
	if (ft_tabmin(moving->y, 4) + moving->y_offset > v->ymax)
	{
		v->side++;
		v->move_engaged = 0;
	}
	else
	{
//		printf("fillit_move_along_right pre-adjustment loop\n");
		while (fillit_check_collision(e, moving)
				|| !fillit_check_contact(e, moving))
		{
//			printf("fillit_move_along_right adjustment loop\n");
//			printf("Collision = %d, contact = %d\n",
//					fillit_check_collision(e, moving),
//					fillit_check_contact(e, moving));
//			printf("Tetri %c : moving->x_offset = %d, moving->y_offset = %d\n",
//					moving->letter, moving->x_offset, moving->y_offset);
			if (fillit_check_collision(e, moving))
				moving->x_offset += 1;
			else if (!fillit_check_contact(e, moving))
				moving->x_offset -= 1;
//			usleep(100000);
		}
	}
//	printf("Tetri %c : moving->x_offset = %d, moving->y_offset = %d\n",
//			moving->letter, moving->x_offset, moving->y_offset);
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
** REVERTED ==============================================================
*/

void	fillit_move_along_left_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->y_offset -= 1;
	if (ft_tabmax(moving->y, 4) + moving->y_offset < v->ymin)
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
//			printf("moving->x_offset = %d\n", moving->x_offset);
//			printf("ft_tabmax(moving->y, 4) = %d\n", ft_tabmax(moving->y, 4));
//			printf("moving->y_offset = %d\n", moving->y_offset);
//			printf("v->ymin = %d\n", v->ymin);

		}
	}
}

void	fillit_move_along_top_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->x_offset -= 1;
	if (ft_tabmax(moving->x, 4) + moving->x_offset < v->xmin)
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

void	fillit_move_along_bottom_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		moving->y_offset = v->ymax;
		v->move_engaged = 1;
	}
	else
		moving->x_offset -= 1;
	if (ft_tabmax(moving->x, 4) + moving->x_offset < v->xmin)
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

void	fillit_move_along_right_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmax;
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->y_offset -= 1;
	if (ft_tabmax(moving->y, 4) + moving->y_offset < v->ymin)
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
				moving->x_offset += 1;
			else if (!fillit_check_contact(e, moving))
				moving->x_offset -= 1;
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
	char	*out_tmp;

//	if (moving->letter > e->letter)
//	{
		if (v->side == 0)
			fillit_move_along_right(e, v, moving);
		else if (v->side == 1)
			fillit_move_along_bottom(e, v, moving);
		else if (v->side == 2)
			fillit_move_along_left(e, v, moving);
		else if (v->side == 3)
			fillit_move_along_top(e, v, moving);
/*	}
	else
	{
		if (v->side == 0)
			fillit_move_along_left_rev(e, v, moving);
		else if (v->side == 1)
			fillit_move_along_top_rev(e, v, moving);
		else if (v->side == 2)
			fillit_move_along_right_rev(e, v, moving);
		else if (v->side == 3)
			fillit_move_along_bottom_rev(e, v, moving);
	}*/
	if (e->result)
	{
		ft_putstr("\033[2J");
		ft_putstr("\n\n");
		fillit_save_printable(e, &e->result);
		ft_putendl(e->result);

		ft_putendl("\n");

		out_tmp = fillit_get_output_map(e);
		ft_putendl(out_tmp);
		ft_strdel(&out_tmp);
		usleep(200000);
	}
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
	int		sq_siz;

	e->inception++;
	fillit_init_vars(&v);
	v.lst_ptr = e->first;
//	debug_inception_print(e);
//	printf("%lu of %lu tetris are locked\n", e->tlocked, e->tcount);
	sq_siz = fillit_square_size(e);
	if (!e->smallest_size || sq_siz < e->smallest_size)
	{
		if (e->tlocked == e->tcount)
		{

			e->smallest_size = sq_siz;
//			ft_putstr("\033[2J");
			printf("\t\t\033[32mThis is the smallest so far (%d)\033[0m\n", e->smallest_size);
			//			printf("Smallest square is %d\n", e->smallest_size);
			if (!e->result)
			{
				e->result = (char *)ft_memalloc(e->smallest_size *
						(e->smallest_size + 1) + 1);
				//				printf("e->result @ %p contains %d bytes\n", e->result,
				//						e->smallest_size * (e->smallest_size + 1) + 1);
			}
			fillit_save_printable(e, &e->result);
			ft_putendl(e->result);
			usleep(500000);
		}
		else
		{
			while (v.lst_ptr)
			{
				//			printf("List element address is %p\n", v.lst_ptr);
				v.tet_ptr = (v.lst_ptr)->content;
				//			printf("\t\t    Tetri %c fixed = %d\n",
				//					v.tet_ptr->letter, v.tet_ptr->fixed);
				//			fillit_print_xy(v.tet_ptr);
				if (!v.tet_ptr->fixed)
				{
					fillit_get_fixed_range(e, &v);
					printf("xmin = %d, xmax = %d\n", v.xmin, v.xmax);
					printf("ymin = %d, ymax = %d\n", v.ymin, v.ymax);
//					fillit_print_all_tetri_status(e);
					while (v.side < 4)
					{
//						printf("v.side = %d\n", v.side);
						//					printf("Check, side = %d\n", v.side);
						fillit_set_position(e, &v, v.tet_ptr);
						//					ft_putstr("set_position OK\n");
						v.tet_ptr->fixed = 1;
						e->prev_letter = e->letter;
						e->letter = v.tet_ptr->letter;
						e->tlocked++;
						fillit_solve(e);

						//					debug_print_t_tetri(e, v.tet_ptr);
						v.tet_ptr->fixed = 0;
						e->letter = e->prev_letter;
						e->tlocked--;
					}

				}
				//			printf("List element address is %p\n", v.lst_ptr);
				//			ft_print_memory(v.lst_ptr, sizeof(t_list));
				//			ft_putendl("\n\t\t\t-----------------\n");
				v.lst_ptr = v.lst_ptr->next;
			}
		}
	}
	e->inception--;
}
