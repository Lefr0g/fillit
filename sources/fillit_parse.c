/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_parse.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:26 by amulin            #+#    #+#             */
/*   Updated: 2016/02/19 18:43:08 by lpoujade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function adds a new element to the end of the list, containing the
** t_tetri structure of a tetrimino.
*/

static int		fillit_new_tetri(t_list **list_ptr, char letter)
{
	t_tetri tmp;

	ft_bzero(tmp.raw, 17);
	tmp.x_offset = 0;
	tmp.y_offset = 0;
	tmp.fixed = 0;
	tmp.letter = letter;
	ft_lstappend(list_ptr, ft_lstnew(&tmp, sizeof(t_tetri)));
	*list_ptr = (*list_ptr)->next;
	return (0);
}

static int		cc(char *buff, int i)
{
	int	count;

	count = (buff[i - 1] == '#') ? 1 : 0;
	count += (buff[i + 1] == '#') ? 1 : 0;
	count += (buff[i - 5] == '#') ? 1 : 0;
	count += (buff[i + 5] == '#') ? 1 : 0;
	return (count);
}

static int		read_p(int fd, char *raw)
{
	int		ret;
	int		bcount;
	int		ccount;
	int		i;
	char	*buff;

	buff = ft_strnew(20);
	if (buff && read(fd, buff, 20) == 20)
	{
		i = -1;
		bcount = 0;
		ccount = 0;
		while (buff[++i] && (buff[i] == '.' || buff[i] == '#' ||
					(buff[i] == '\n' && ((i + 1) / 5))))
			if (buff[i] != '\n' && (*raw++ = buff[i]))
				if (buff[i] == '#' && ++bcount)
					ccount += cc(buff, i);
		ret = (read(fd, buff, 1) == 0) ? 1 : 0;
		if (bcount != 4 || ccount < 6 || i != 20 || (!ret && *buff != '\n'))
			ret = 2;
	}
	else
		ret = 2;
	ft_memdel((void **)&buff);
	return (ret);
}

int				fillit_parse(t_env *e, int fd)
{
	t_list	*lst;
	int		ret;
	int		read_ret;
	char	letter;

	if (fd < 3 || read(fd, NULL, 0) == -1)
		return (-1);
	letter = 'A';
	lst = e->first;
	ret = 0;
	while (!(read_ret = read_p(fd, ((t_tetri*)lst->content)->raw)))
	{
		letter++;
		fillit_new_tetri(&lst, letter);
	}
	if (read_ret != 1)
		ret = 1;
	if (!ret)
		fillit_load_xy(e);
	close(fd);
	return (ret);
}
