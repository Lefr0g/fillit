/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2015/12/29 15:44:56 by amulin           ###   ########.fr       */
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
*/
int	fillit_blocks_check(char *raw)
{
	int	i;
	int	blocks;

	i = 0;
	blocks = 0;
	while (i < 17)
	{
		if (raw[i] == '#')
		{
			if ((i - 1 > 0 && raw[i - 1] == '#')
					|| (i + 1 < 17 && raw[i + 1] == '#')
					|| (i - 4 > 0 && raw[i - 4] == '#')
					|| (i + 4 < 17 && raw[i + 4] == '#'))
			blocks++;
		else
			return (-1);
		}
		i++;
	}
	return (blocks);
}

/*
** This function simply checks that the current 1/4 of tetrimino only contains
** allowed characters, and that the length is correct.
** At the begining the 'jump' and 'layers' variables are reset in case we are
** starting to parse a new tetrimino.
*/
int	fillit_line_check(t_tmp *tmp)
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
	return (blocks);
}

int	fillit_input_check(t_env *e, int fd)
{
	int		blocks;
	int		tet_siz;
	t_list	*list_ptr;
	t_tetri	*tetri_ptr;

	tet_siz = sizeof(t_tetri);
	blocks = 0;
	e->tmp->jump = 0;
	e->tmp->layers = 0;
//	Get pointers to t_terti and the first list element :
	list_ptr = e->first;
	tetri_ptr = (t_tetri*)(list_ptr->content);

	while ((e->tmp->gnl_ret = get_next_line(fd, &e->tmp->line)) != 0)
	{
		if (e->tmp->gnl_ret == -1)
			return (fillit_error("gnl fail"));
		if (e->tmp->line[0])
		{
			if ((e->tmp->linecheck_ret = fillit_line_check(e->tmp)) < 0)
				return (e->tmp->linecheck_ret);
			else
			{
				blocks += e->tmp->linecheck_ret;
//				Then concatenate 'line' to t_tetri 'raw' string :
				if (ft_strlen(tetri_ptr->raw) <= 12)
					ft_strncat(tetri_ptr->raw, e->tmp->line, 4);
			}
		}
		else
		{
			e->tmp->jump++;
			if (blocks != 4 && e->tmp->jump == 1)
				return (fillit_error("not enough / too many blocks in tetri"));
			else
			{
//				Check validity current tetrimino (search isolated blocks) :
				if (fillit_blocks_check(tetri_ptr->raw) == -1)
					return (fillit_error("invalid block placement within tetri"));
//				Then create a new t_tetri (list element) :
				ft_lstappend(&(e->first), ft_lstnew(malloc(tet_siz), tet_siz));
				list_ptr = list_ptr->next;
				tetri_ptr = (t_tetri*)(list_ptr->content);
			}
			blocks = 0;
		}
		if (e->tmp->jump && e->tmp->layers != 4)
			return (fillit_error("wrong tetri height"));
		if (e->tmp->jump > 1)
			return (fillit_error("more than one empty line"));
//		ft_putendl(tmp->line);
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
	ret = fillit_input_check(e, e->tmp->fd);
	while (get_next_line(e->tmp->fd, &e->tmp->line))
		(void)e;
	close(e->tmp->fd);
	return (ret);
}
