/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2016/02/11 20:35:58 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function adds a new element to the end of the list, containing the
** t_tetri structure of a tetrimino.
*/

int	fillit_new_tetri(t_list **list_ptr, t_tetri **tetri_ptr)
{
	t_tetri tmp;

	ft_lstappend(list_ptr, ft_lstnew(&tmp, sizeof(t_tetri)));
	*list_ptr = (*list_ptr)->next;
	*tetri_ptr = (t_tetri*)((*list_ptr)->content);
	ft_bzero((*tetri_ptr)->raw, 17);
	(*tetri_ptr)->x_offset = 0;
	(*tetri_ptr)->y_offset = 0;
	(*tetri_ptr)->fixed = 0;
	return (0);
}

/*
** This is the parsing function called by main.
** It opens the file and then calls the fillit_input_check() function.
** Notice that the get_next_line() loop call below is used to free the static
** 'keep' string variable within gnl() in cases where an error is found in the
** input file and fillit_input_check() stops prematurely.
** The error detection for empty line at the end of the input file is done at
** this stage. If an error is detected during fillit_input_check(), the 'jump'
** variable is reset to avoid false positive / double errors.
*/

int	fillit_parse(t_env *e, char *filename)
{
	int	ret;

	if (ft_basicfilecheck(filename, '\n', 5) != 1)
		return (-1);
	e->tmp->fd = open(filename, O_RDONLY);
	if ((e->tmp->fd = open(filename, O_RDONLY)) == -1)
		return (fillit_error("open() failed", DEBUG_MODE));
	if (DEBUG_MODE)
		printf("OPEN SUCCESS, fd = %d\n", e->tmp->fd);
	ret = fillit_input_check(e);
	if (e->tmp->jump > 1)
		return (fillit_error("empty line at the end of the file", DEBUG_MODE));
	if (ret != -1)
	{
		fillit_load_xy(e);
		if (DEBUG_MODE)
			fillit_print_raw(e);
		close(e->tmp->fd);
	}
	return (ret);
}
