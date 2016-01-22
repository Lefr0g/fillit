/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_parse.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2016/01/22 12:21:40 by liumsade         ###   ########.fr       */
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
	(*tetri_ptr)->firstmove = 1;
	return (0);
}

/*
** This function checks that the tetrimino blocks are correctly placed.
** A one-line representation of the full tetrimino is used.
** For example, this:
**  ......#..##..#..
** is a correct tetrimino, corresponding to
**  ....
**  ..#.
**  .##.
**  .#..
** But this one:
**  ..#..##......#..
** is incorrect since one of its blocks is isolated form the others.
**  ..#.
**  .##.
**  ....
**  .#..
** How it works :
** At the beginning, we know that an empty line has just been encountered.
** First we count how many layers the previous tetrimino has.
** Then we use the raw char* data to check that the blocks are correctly
** positioned.
** Finally, we add a new t_tetri within a new t_list element for the next
** tetrimino. Then we reset the block counter.
*/

int	fillit_blocks_check(t_env *e, t_tetri *tetri_ptr)
{
	int	i;

	i = -1;
	e->tmp->jump++;
	if (e->tmp->blocks != 4 && e->tmp->jump == 1)
		return (-1);
	else
	{
		while (++i < 17)
			if (tetri_ptr->raw[i] == '#')
			{
				if (!((i - 1 >= 0 && tetri_ptr->raw[i - 1] == '#')
						|| (i + 1 < 17 && tetri_ptr->raw[i + 1] == '#')
						|| (i - 4 > 0 && tetri_ptr->raw[i - 4] == '#')
						|| (i + 4 < 17 && tetri_ptr->raw[i + 4] == '#')))
					return (-1);
			}
	}
	e->tmp->blocks = 0;
	return (0);
}

/*
** This function simply checks that the current 1/4 of tetrimino only contains
** allowed characters, and that the length is correct.
** At the begining the 'jump' and 'layers' variables are reset in case we are
** starting to parse a new tetrimino.
*/

int	fillit_layer_check(t_tmp *tmp, t_list **list_ptr, t_tetri **tetri_ptr)
{
	int	blocks;

	blocks = 0;
	if (tmp->jump)
	{
		tmp->jump = 0;
		tmp->layers = 0;
		fillit_new_tetri(list_ptr, tetri_ptr);
	}
	tmp->i = 0;
	while (tmp->line[tmp->i])
	{
		if (tmp->line[tmp->i] != '.' && tmp->line[tmp->i] != '#')
			return (-1);
		else if (tmp->line[tmp->i] == '#')
			blocks++;
		(tmp->i)++;
	}
	if (tmp->i != 4)
		return (-1);
	(tmp->layers)++;
	(tmp->blocks) += blocks;
	if (ft_strlen((*tetri_ptr)->raw) <= 12)
		ft_strncat((*tetri_ptr)->raw, tmp->line, 4);
	return (blocks);
}

/*
** A loop call to gnl() is made until the end of the file read. It can be
** interrupted if any issue is detected in this file, as defined in the project
** specifications.
** When a non-empty line is read, fillit_layer_check() is called.
** If the line is empty, it sould mean that a tetrimino has been fully read. In
** this case the fillit_block_check function is called to verify that the '#'
** blocks are placed correctly, ensuring a valid tetrimino.
** The two last checks ensure that only 4-layers high tetriminos are accepted,
** and that two tetriminos can only be separated by one empty line.
** This function returns 0 or -1 to fillit_parse().
** list_ptr and tetri_ptr are simple shortcuts to env substructures.
*/

int	fillit_input_check(t_env *e)
{
	t_list	*list_ptr;
	t_tetri	*tetri_ptr;

	list_ptr = e->first;
	tetri_ptr = (t_tetri*)(list_ptr->content);
	while ((e->tmp->gnl_ret = get_next_line(e->tmp->fd, &e->tmp->line)) != 0)
	{
		if (e->tmp->gnl_ret == -1)
			return (-1);
		if (e->tmp->line[0])
		{
			if ((fillit_layer_check(e->tmp, &list_ptr, &tetri_ptr)) < 0)
				return (-1);
		}
		else
		{
			if (fillit_blocks_check(e, tetri_ptr) < 0)
				return (-1);
		}
		if (e->tmp->jump && e->tmp->layers != 4)
			return (-1);
		if (e->tmp->jump > 1)
			return (-1);
	}
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

	e->tmp->fd = open(filename, O_RDONLY);
	if ((e->tmp->fd = open(filename, O_RDONLY)) == -1)
		return (-1);
	ret = fillit_input_check(e);
	if (ret == -1)
	{
		e->tmp->jump = 0;
		while (get_next_line(e->tmp->fd, &e->tmp->line))
			(void)ret;
		return (ret);
	}
	if (e->tmp->jump)
		return (-1);
	fillit_load_xy(e);
	close(e->tmp->fd);
	return (ret);
}
