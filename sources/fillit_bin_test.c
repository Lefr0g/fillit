/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_bin_test.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liums <liums@openaliasbox.org>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/09 13:41:47 by liums             #+#    #+#             */
/*   Updated: 2016/01/10 21:20:48 by lpoujade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
 * new data representation : each tetriminos is a 4-octets number
*/

static	unsigned short	magic(char *raw)
{
	int			i;
	int			len;
	unsigned short ret;

	ret = 0;
	i = ft_strlen(raw);
	len = i - 1;
	while (i--)
	{
		if (raw[i] == '#')
			ret |= 1 << (len - i);
	}
	return (ret);
}

static void		fillit_bin_print_raw(t_tetri *p);
void		fillit_raw2binary(t_env *e)
{
	t_list	*l_lst;
	t_tetri *t_ptr;

	l_lst = e->first;
	t_ptr = (t_tetri *)l_lst->content;

		ft_putendl("(letter / decimal / hexa / raw = binary)\n");
	while (l_lst)
	{
		t_ptr->binary_map = magic(t_ptr->raw);

		ft_putchar(t_ptr->letter);
		ft_putchar('\t');
		ft_putnbr(t_ptr->binary_map);
		ft_putchar('\t');
		ft_puthex(t_ptr->binary_map, "min");
		ft_putchar('\t');
		fillit_bin_print_raw(t_ptr);
		ft_putchar('\n');

		if ((l_lst = l_lst->next))
			t_ptr = (t_tetri *)l_lst->content;
	}
}

/*
 * Place all tetris to top-left corner in binary_map
*/

void	fillit_bin_place(t_env *e)
{
	t_tetri *mov;
	t_list	*lst;

	lst = e->first;
	mov = (t_tetri *)lst->content;
	while (lst)
	{
		while (mov->binary_map < 4096)
			mov->binary_map = mov->binary_map << 4;
		if ((lst = lst->next))
			mov = (t_tetri *)lst->content;
	}
}

static void	fillit_bin_print_raw(t_tetri *p)
{
	int	c;

	c = 16;
	while (c)
	{
		if (!(c % 4) && c != 16)
			ft_putchar(' ');
		if (1 & (p->binary_map >> (c-1)))
			ft_putchar('1');
		else
			ft_putchar('0');
		c--;
	}
}

/*
 * Return str of fixed tetrimino using tetri->binary_map
*/

char	*fillit_bin_print(t_env *e)
{
	t_tetri *mov;
	t_list	*lst;
	char	*ret;
	int		siz;
	int		c;

	siz = e->smallest_size;
	ret = (char *)malloc(siz * (siz + 1) + 1);
	siz = siz * (siz + 1) - 1;
	ft_memset(ret, '.', siz);
	while (siz > 0)
	{
		ret[siz] = '\n';
		siz -= e->smallest_size + 1;
	}
	ret[e->smallest_size * ( e->smallest_size + 1) + 1] = 0;
	lst = e->first;
	mov = (t_tetri *)lst->content;
	while (lst)
	{
		if (mov->letter == 'B')
			mov->fixed = 1;
		else
			mov->fixed = 0;
		if (mov->fixed)
		{
			c = (mov->x_offset + mov->y_offset * (e->smallest_size + 1)) + 16;
			while (c)
			{
				if (ret[c] == '\n')
					c--;
				if (1 & (mov->binary_map >> (c-1)))
					ret[c] = mov->letter;
				c--;
			}
		}
		if ((lst = lst->next))
			mov = (t_tetri *)lst->content;
	}
	return (ret);
}
