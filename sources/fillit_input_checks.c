/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_input_checks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2016/02/17 18:54:29 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

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
	if (e->tmp->layers != 4)
		return (fillit_error("wrong tetri height", DEBUG_MODE));
	if (e->tmp->blocks != 4 && e->tmp->jump == 1)
		return (fillit_error("not enough / too many blocks in tetri",
					DEBUG_MODE));
	else
	{
		if (fillit_blocks_check_sub(tetri_ptr))
			return (-1);
	}
	e->tmp->blocks = 0;
	return (0);
}

/*
** Subfunction of fillit_blocks_check for norme
*/

int	fillit_blocks_check_sub(t_tetri *tet_p)
{
	int	i;
	int	score;

	i = -1;
	score = 0;
	while (++i < 16)
	{
		if (tet_p->raw[i] == '#')
		{
			if (i + 1 < 16 && tet_p->raw[i + 1] == '#')
				(score)++;
			if (i + 4 < 16 && tet_p->raw[i + 4] == '#')
				(score)++;
			if (i - 1 >= 0 && tet_p->raw[i - 1] == '#')
				(score)++;
			if (i - 4 >= 0 && tet_p->raw[i - 4] == '#')
				(score)++;
		}
	}
	if (score < 6)
		return (fillit_error("invalid block placement", DEBUG_MODE));
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
			return (fillit_error("forbidden character", DEBUG_MODE));
		else if (tmp->line[tmp->i] == '#')
			blocks++;
		(tmp->i)++;
	}
	if (tmp->i != 4)
		return (fillit_error("wrong line size", DEBUG_MODE));
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
** If the line is empty, it should mean that a tetrimino has been fully read. In
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
		if (fillit_input_check_sub(e, &list_ptr, &tetri_ptr) == -1)
			return (-1);
	}
	if (e->tmp->layers)
	{
		if (fillit_blocks_check(e, tetri_ptr) < 0)
			return (-1);
	}
	return (0);
}

/*
** Subfunction of fillit_input_check() for norme.
*/

int	fillit_input_check_sub(t_env *e, t_list **list_ptr, t_tetri **tetri_ptr)
{
	if (e->tmp->gnl_ret == -1)
		return (fillit_error("gnl fail", DEBUG_MODE));
	if (e->tmp->line[0])
	{
		if ((fillit_layer_check(e->tmp, list_ptr, tetri_ptr)) < 0)
			return (-1);
	}
	else
	{
		e->tmp->jump++;
		if (fillit_blocks_check(e, *tetri_ptr) < 0)
			return (-1);
	}
	if ((e->tmp->jump && e->tmp->layers != 4)
			|| (!e->tmp->jump && e->tmp->layers > 4))
		return (fillit_error("wrong tetri height", DEBUG_MODE));
	if (e->tmp->jump > 1)
		return (fillit_error("more than one empty line", DEBUG_MODE));
	return (0);
}
