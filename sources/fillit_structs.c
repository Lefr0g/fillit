/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_structs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/20 15:29:05 by amulin            #+#    #+#             */
/*   Updated: 2016/01/28 16:37:46 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** Allocate and initialize the environment and related sub-structures.
*/

int		fillit_init(t_env **e)
{
	t_tetri		tet_ptr;

	tet_ptr.letter = '\0';
	tet_ptr.x_offset = 0;
	tet_ptr.y_offset = 0;
	tet_ptr.fixed = 0;
	ft_bzero(tet_ptr.raw, 17);
	if (!(*e = (t_env*)malloc(sizeof(t_env))))
		return (-1);
	if (!((*e)->tmp = (t_tmp*)malloc(sizeof(t_tmp))))
		return (-1);
	if (!((*e)->tmp->line = ft_strnew(10)))
		return (-1);
	fillit_reset_tmp((*e)->tmp);
	(*e)->tcount = 0;
	(*e)->update = 0;
	(*e)->color = 0;
	(*e)->smallest_size = 0;
	ft_bzero((*e)->set, 25);
	if (!((*e)->first = ft_lstnew(&tet_ptr, sizeof(t_tetri))))
		return (-1);
	return (0);
}

/*
** Initialize the t_tmp substructure.
*/

void	fillit_reset_tmp(t_tmp *tmp)
{
	tmp->i = 0;
	tmp->fd = 0;
	tmp->gnl_ret = 0;
	tmp->jump = 0;
	tmp->layers = 0;
	tmp->layercheck_ret = 0;
	tmp->blocks = 0;
	ft_bzero(tmp->line, sizeof(tmp->line));

}

/*
** Initialize the t_vars local structure
*/

void	fillit_init_vars(t_vars *v)
{
	v->i = 0;
	v->isaved = -1;
	v->height = -1;
	v->pos = 0;
	v->side = 0;
	v->move_engaged = 0;
	v->xmax = INT_MIN;
	v->xmin = INT_MAX;
	v->ymax = INT_MIN;
	v->ymin = INT_MAX;
	v->lst_ptr = NULL;
	v->lst_mov = NULL;
	v->lst_fix = NULL;
	v->tet_ptr = NULL;
	v->tet_mov = NULL;
	v->tet_fix = NULL;
}

/*
** Initialize some environment variables that are used by some function instead
** of local variables / counters for norme reasons.
*/

void	fillit_reset_quickvars(t_env *e)
{
	e->block = 0;
	e->i = 0;
	e->j = 0;
	e->x = 0;
	e->y = 0;
}

/*
** Delete a tetrimino element within a list element.
** Called as a function pointer by ft_lstdel()
*/

void	fillit_del_tetri(void *content, size_t size)
{
	ft_bzero(content, size);
	free((t_tetri*)content);
}
