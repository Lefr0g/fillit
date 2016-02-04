/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/03 19:07:10 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int		fillit_new_is_better(t_env *e)
{
	t_list	*oldlist;
	t_tetri	*oldtet;
	t_list	*newlist;
	t_tetri	*newtet;
	int		diff;

	oldlist = e->first;
	newlist = e->copy;
	while (newlist && oldlist)
	{
		oldtet = (t_tetri*)oldlist->content;
		newtet = (t_tetri*)newlist->content;

		diff = (newtet->x_offset - oldtet->x_offset) +
			(newtet->y_offset - oldtet->y_offset);
		if (diff < 0)
			return (1);
		oldlist = oldlist->next;
		newlist = newlist->next;
	}
	return (0);
}

t_list	*fillit_lstcpy_joke(t_list *elem)
{
	return (elem);
}

t_list	*fillit_copy_list(t_list *input)
{
	return (ft_lstmap(input, &fillit_lstcpy_joke));
}

int		fillit_can_be_smaller(t_env *e)
{
	int		free;
//	char	*outmap;

	free = (e->smallest_size * e->smallest_size) - (e->tcount * 4);
	if (free >= e->smallest_size * 2 - 1)
	{
//		printf("Can be smaller\n");
//		outmap = fillit_get_output_map(e);
//		ft_putendl(outmap);
//		ft_strdel(&outmap);
		return (1);
	}
//	printf("Cannot be smaller\n");
//	outmap = fillit_get_output_map(e);
//	ft_putendl(outmap);
//	ft_strdel(&outmap);
	return (0);
}

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
	{//	printf("Tetri %c : moving->x_offset = %d, moving->y_offset = %d\n",
//			moving->letter, moving->x_offset, moving->y_offset);
//	printf("fillit_move_along_right END\n");

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
	usleep(500000);

}

void	debug_print_map(t_env *e)
{
	char	*out_tmp;
	
	ft_putstr("\033[2J");
	ft_putstr("\n\n");
	/*
	if (e->result)
	{
		fillit_save_printable(e, &e->result);
		ft_putendl(e->result);

		ft_putendl("\n");
	}
	*/

	out_tmp = fillit_get_output_map(e);
	ft_putendl(out_tmp);
	if (e->tlocked == e->tcount)
		ft_putstr("\033[32m");
	else
		ft_putstr("\033[33m");
	printf("%lu locked / %lu total\n\n", e->tlocked, e->tcount);
	ft_putstr("\033[0m");
	ft_strdel(&out_tmp);
	usleep(ANIMATION_DELAY);
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
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along RIGHT\n");
	if (!v->move_engaged)
	{
		moving->y_offset = v->ymin;
//		printf("Positionning %c at top-left :\n", moving->letter);
//		printf("\tmoving->x_offset = %d, moving->y_offset = %d\n",
//				moving->x_offset, moving->y_offset);
		v->move_engaged = 1;
	}
	else
		moving->y_offset += 1;
	moving->x_offset = v->xmin - ft_tabmax(moving->x, 4) - 1;
	//	printf("fillit_move_along_right MIDDLE\n");

	//		printf("fillit_move_along_right pre-adjustment loop\n");
	while (!(!fillit_check_collision(e, moving) &&
			(fillit_check_contact(e, moving) && touch)))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->x_offset += 1;
//		if (DEBUG_MODE)
//			debug_print_map(e);
	}
	if (ft_tabmin(moving->y, 4) + moving->y_offset >= v->ymax)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

void	fillit_move_along_bottom(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along BOTTOM\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin - ft_tabmax(moving->x, 4);
		v->move_engaged = 1;
	}
	else
		moving->x_offset += 1;
	moving->y_offset = v->ymin - ft_tabmax(moving->y, 4) - 1;
	while (!(!fillit_check_collision(e, moving)
			&& (fillit_check_contact(e, moving) && touch)))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->y_offset += 1;
	}
	if (ft_tabmin(moving->x, 4) + moving->x_offset >= v->xmax)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

void	fillit_move_along_left(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along LEFT\n");
	if (!v->move_engaged)
	{
		moving->y_offset = v->ymin - ft_tabmax(moving->y, 4);
		v->move_engaged = 1;
	}
	else
		moving->y_offset += 1;
	moving->x_offset = v->xmax + 1;

	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->x_offset -= 1;
	}
	if (ft_tabmin(moving->y, 4) + moving->y_offset >= v->ymax)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

void	fillit_move_along_top(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along TOP\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin - ft_tabmax(moving->x, 4);
		v->move_engaged = 1;
	}
	else
		moving->x_offset += 1;
	moving->y_offset = v->ymax + 1;

	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->y_offset -= 1;
	}
	if (ft_tabmin(moving->x, 4) + moving->x_offset >= v->xmax)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

/*
** REVERTED ==============================================================
*/

void	fillit_move_along_left_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along LEFT (REV)\n");
	if (!v->move_engaged)
	{
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->y_offset -= 1;
	moving->x_offset = v->xmax + 1;

	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->x_offset -= 1;
	}
	if (ft_tabmax(moving->y, 4) + moving->y_offset <= v->ymin)
	{
		v->side++;
		v->move_engaged = 0;
	}
}


void	fillit_move_along_top_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along TOP (REV)\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmin;
		v->move_engaged = 1;
	}
	else
		moving->x_offset -= 1;
	moving->y_offset = v->ymax + 1;
	
	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->y_offset -= 1;
	}
	if (ft_tabmax(moving->x, 4) + moving->x_offset <= v->xmin)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

void	fillit_move_along_bottom_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along BOTTOM (REV)\n");
	if (!v->move_engaged)
	{
		moving->x_offset = v->xmax;
		v->move_engaged = 1;
	}
	else
		moving->x_offset -= 1;
	moving->y_offset = v->ymin - ft_tabmax(moving->y, 4) - 1;

	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->y_offset += 1;
	}
	if (ft_tabmax(moving->x, 4) + moving->x_offset <= v->xmin)
	{
		v->side++;
		v->move_engaged = 0;
	}
}

void	fillit_move_along_right_rev(t_env *e, t_vars *v, t_tetri *moving)
{
	int	touch;

	touch = 0;
	if (DEBUG_MODE)
		printf("fillit_move_along RIGHT (REV)\n");
	if (!v->move_engaged)
	{
		moving->y_offset = v->ymin;
		v->move_engaged = 1;
	}
	else
		moving->y_offset -= 1;
	moving->x_offset = v->xmin - ft_tabmax(moving->x, 4) - 1;

	while (!(!fillit_check_collision(e, moving)
			&& fillit_check_contact(e, moving) && touch))
	{
		if (fillit_check_collision(e, moving))
			touch = 1;
		moving->x_offset += 1;
	}
	if (ft_tabmax(moving->y, 4) + moving->y_offset <= v->ymin)
	{
		v->side++;
		v->move_engaged = 0;
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
	printf("Prev = %c, Current = %c\n", v->prev_letter, v->curr_letter);
	if (v->curr_letter > v->prev_letter)
	{
		if (v->side == 0)
			fillit_move_along_right(e, v, moving);
		else if (v->side == 1)
			fillit_move_along_bottom(e, v, moving);
//		else if (v->side >= 2 && !fillit_can_be_smaller(e))
//			v->side = 4;
		else if (v->side == 2)
			fillit_move_along_left(e, v, moving);
		else if (v->side == 3)
			fillit_move_along_top(e, v, moving);
//	/*	
	}
	else
	{
		if (v->side == 0)
			fillit_move_along_left_rev(e, v, moving);
		else if (v->side == 1)
			fillit_move_along_top_rev(e, v, moving);
//		else if (v->side >= 2 && !fillit_can_be_smaller(e))
//			v->side = 4;
//		else if (v->side == 2)
//			v->side = 4;
		else if (v->side == 2)
			fillit_move_along_bottom_rev(e, v, moving);
		else if (v->side == 3)
			fillit_move_along_right_rev(e, v, moving);
	}
//	*/
}




/*
** 0/ Se mettre en debut de liste
**   1/ Positionner le tetri
**   2/ Immobiliser le tetri
**   3/ Lancer recursivement jusqu'a ce que tous les tetris soient fixes
** 4/ Deverrouiller le tetri
** 5/ Selectionner le tetri suivant, retour en 1/ jusqu'au dernier tetri
*/

void	fillit_solve(t_env *e, char latest_letter)
{
	t_vars	v;
	int		sq_siz;

	e->inception++;
	fillit_init_vars(&v);
	v.lst_ptr = e->first;
//	debug_inception_print(e);
//	printf("%lu of %lu tetris are locked\n", e->tlocked, e->tcount);
	sq_siz = fillit_square_size(e);
	if (!e->smallest_size || sq_siz <= e->smallest_size)
	{
//		if (e->tlocked == e->tcount)
		if (e->tlocked == e->tcount && (!e->smallest_size
					|| sq_siz <= e->smallest_size))
		{

			e->smallest_size = sq_siz;
//			ft_putstr("\033[2J");
			if (DEBUG_MODE)
				printf("\t\t\033[32mThis is the smallest so far (%d)\033[0m\n",\
						e->smallest_size);
			//			printf("Smallest square is %d\n", e->smallest_size);
			if (!e->result)
			{
				e->result = (char *)ft_memalloc(e->smallest_size *
						(e->smallest_size + 1) + 1);
//				printf("e->result @ %p contains %d bytes\n", e->result,
//						e->smallest_size * (e->smallest_size + 1) + 1);
			}
			if (e->copy)
			{
				printf("Deleting copied list...");
				ft_lstdel(&e->copy, &ft_bzero);
				printf(" Done\n");
			}
			if (e->first)
				e->copy = fillit_copy_list(e->first);
			if (!e->result[0] || fillit_new_is_better(e))
			{
				printf("New solution is better\n");
				fillit_save_printable(e, &e->result);
			}
			if (DEBUG_MODE)
			{
				ft_putendl(e->result);
			}
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
//					printf("xmin = %d, xmax = %d\n", v.xmin, v.xmax);
//					printf("ymin = %d, ymax = %d\n", v.ymin, v.ymax);
//					fillit_print_all_tetri_status(e);
					while (v.side < 4)
					{
						if (!v.curr_letter)
							v.prev_letter = latest_letter;
						else
							v.prev_letter = v.curr_letter;
						v.curr_letter = v.tet_ptr->letter;
//						printf("curr = %c, prev = %c\n", v.curr_letter, v.prev_letter);
//						printf("v.side = %d\n", v.side);
//						printf("Check, side = %d\n", v.side);
						fillit_set_position(e, &v, v.tet_ptr);
//						ft_putstr("set_position OK\n");
						v.tet_ptr->fixed = 1;
						

						e->tlocked++;
						if (DEBUG_MODE)
							debug_print_map(e);
						fillit_solve(e, v.tet_ptr->letter);

//						debug_print_t_tetri(e, v.tet_ptr);
						v.tet_ptr->fixed = 0;
						v.curr_letter = v.prev_letter;
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
