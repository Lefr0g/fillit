/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2015/12/28 16:49:29 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int	fillit_line_check(t_tmp *tmp)
{
	int	blocks;

	blocks = 0;
	if (tmp->jump)
	{
		tmp->jump = 0;
		tmp->i = 0;
	}
	tmp->j = 0;
	while (tmp->line[tmp->j])
	{
		if (tmp->line[tmp->j] != '.' && tmp->line[tmp->j] != '#')
			return (fillit_error("forbidden character"));
		else if (tmp->line[tmp->j] == '#')
			blocks++;
		(tmp->j)++;
	}
	if (tmp->j != 4)
		return (fillit_error("wrong line size"));
	(tmp->i)++;
	return (blocks);
}

int	fillit_input_check(t_env *e, int fd)
{
	int	blocks;

	blocks = 0;
	e->tmp->jump = 0;
	e->tmp->i = 0;
	while ((e->tmp->gnl_ret = get_next_line(fd, &e->tmp->line)) != 0)
	{
		if (e->tmp->gnl_ret == -1)
			return (fillit_error("gnl fail"));
		if (e->tmp->line[0])
		{
			if ((e->tmp->linecheck_ret = fillit_line_check(e->tmp)) < 0)
				return (e->tmp->linecheck_ret);
			else
				blocks += e->tmp->linecheck_ret;
		}
		else
		{
			e->tmp->jump++;
			if (blocks != 4 && e->tmp->jump == 1)
				return (fillit_error("not enough / too many blocks in tetri"));
			blocks = 0;
		}
		if (e->tmp->jump && e->tmp->i != 4)
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

	printf("pre-open check, filename = %s\n", filename);
	e->tmp->fd = open(filename, O_RDONLY);
	printf("open = %d\n", e->tmp->fd);
	if ((e->tmp->fd = open(filename, O_RDONLY)) == -1)
		return (fillit_error("open() failed"));
	printf("open OK\n");
	ret = fillit_input_check(e, e->tmp->fd);
	while (get_next_line(e->tmp->fd, &e->tmp->line))
		(void)e;
	close(e->tmp->fd);
	return (ret);
}


