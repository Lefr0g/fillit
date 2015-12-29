/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2015/12/29 17:10:39 by amulin           ###   ########.fr       */
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
**
** But this one:
**  ..#..##......#..
** is incorrect since one of its blocks is isolated form the others.
**  ..#.
**  .##.
**  ....
**  .#..
**
** How it works :
** At the beginning, we know that an empty line has just been encountered.
** First we count how many layers the previous tetrimino has.
** Then we use the raw char* data to check that the blocks are correctly
** positioned.
** Finally, we add a new t_tetri within a new t_list element for the next
** tetrimino. Then we reset the block counter.
*/
int	fillit_blocks_check(t_env *e, t_list **list_ptr, t_tetri **tetri_ptr)
{
	int	i;

	i = 0;
	e->tmp->jump++;
	if (e->tmp->blocks != 4 && e->tmp->jump == 1)
		return (fillit_error("not enough / too many blocks in tetri"));
	else
	{
		while (i < 17)
		{
			if ((*tetri_ptr)->raw[i] == '#')
			{
				if (!((i - 1 > 0 && (*tetri_ptr)->raw[i - 1] == '#')
						|| (i + 1 < 17 && (*tetri_ptr)->raw[i + 1] == '#')
						|| (i - 4 > 0 && (*tetri_ptr)->raw[i - 4] == '#')
						|| (i + 4 < 17 && (*tetri_ptr)->raw[i + 4] == '#')))
					return (fillit_error("invalid block placement in tetri"));
			}
			i++;
		}
		ft_lstappend(&(e->first), ft_lstnew(malloc(e->tmp->tet_siz),
				e->tmp->tet_siz));
		*list_ptr = (*list_ptr)->next;
		*tetri_ptr = (t_tetri*)((*list_ptr)->content);
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
int	fillit_layer_check(t_tmp *tmp, t_tetri *tetri_ptr)
{
	int	blocks;

	blocks = 0;
	if (tmp->jump)
	{
		tmp->jump = 0;
		tmp->layers = 0;
	}
	tmp->i = 0;
	while (tmp->line[tmp->i])
	{
		if (tmp->line[tmp->i] != '.' && tmp->line[tmp->i] != '#')
			return (fillit_error("forbidden character"));
		else if (tmp->line[tmp->i] == '#')
			blocks++;
		(tmp->i)++;
	}
	if (tmp->i != 4)
		return (fillit_error("wrong line size"));
	(tmp->layers)++;
	(tmp->blocks) += blocks;
	if (ft_strlen(tetri_ptr->raw) <= 12)
		ft_strncat(tetri_ptr->raw, tmp->line, 4);
	return (blocks);
}
/*
** list_ptr and tetri_ptr are simple shortcuts to env substructures.
**
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
			return (fillit_error("gnl fail"));
		if (e->tmp->line[0])
		{
			if ((e->tmp->layercheck_ret =
						fillit_layer_check(e->tmp, tetri_ptr)) < 0)
				return (e->tmp->layercheck_ret);
		}
		else
		{
			if (fillit_blocks_check(e, &list_ptr, &tetri_ptr) == -1)
				return (-1);
		}
		if (e->tmp->jump && e->tmp->layers != 4)
			return (fillit_error("wrong tetri height"));
		if (e->tmp->jump > 1)
			return (fillit_error("more than one empty line"));
	}
	return (0);
}

/*
** Notice that the get_next_line() loop call below is used to free the static 
** 'keep' string variable within gnl() in cases where an error is found in the 
** input file and fillit_input_check() stops prematurely.
*/
int	fillit_parse(t_env *e, char *filename)
{
	int	ret;

//	ft_putstr("pre-open check, filename = ");
//	ft_putendl(filename);
	e->tmp->fd = open(filename, O_RDONLY);
	if ((e->tmp->fd = open(filename, O_RDONLY)) == -1)
		return (fillit_error("open() failed"));
//	ft_putendl("open OK");
	ret = fillit_input_check(e);
	while (get_next_line(e->tmp->fd, &e->tmp->line))
		(void)e;
	close(e->tmp->fd);
	return (ret);
}
