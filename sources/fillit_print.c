/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/fillit_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2016/02/12 17:49:59 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

void	fillit_colorprint(char *map)
{
	int		i;
	int		color;

	i = 0;
	while (map[i])
	{
		ft_putstr("\x1b[0m");
		if (ft_isalpha(map[i]))
		{
			color = map[i] - 34;
			ft_putstr("\x1b[");
			ft_putnbr(color);
			ft_putchar('m');
		}
		ft_putchar(map[i]);
		i++;
	}
	ft_putstr("\x1b[0m");
}

/*
** This funtion prints a standard or specified error message on the error
** output.
** -------------------------- To be added to libft ----------------------------
*/

int		fillit_error(char *str, int mode)
{
	if (!str || !mode)
	{
		ft_putstr("error");
		ft_putstr("\n");
	}
	else
	{
		ft_putstr_fd("error : ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
	}
	if (!DEBUG_MODE)
		exit(0);
	return (-1);
}

/*
** Subfunction of fillit_get_output_map()
*/

void	fillit_fill_output_map(t_vars *v, char *out)
{
	while (v->lst_ptr)
	{
		v->tet_ptr = (t_tetri*)v->lst_ptr->content;
		v->i = 0;
		while (v->i < 4 && v->tet_ptr->fixed)
		{
			out[(v->tet_ptr->x[v->i] + v->tet_ptr->x_offset - v->xmin)
				+ ((v->tet_ptr->y[v->i] + (v->tet_ptr->y_offset) - v->ymin)
						* (v->side + 1))] = v->tet_ptr->letter;
			v->i++;
		}
		v->lst_ptr = v->lst_ptr->next;
	}
}

/*
** Allocates memory and translates the tetri positions into a char*,
** that can then be printed on stdout
*/

char	*fillit_get_output_map(t_env *e)
{
	t_vars	v;
	char	*out;

	fillit_init_vars(&v);
	fillit_get_fixed_range(e, &v);
	if (v.xmax - v.xmin > v.ymax - v.ymin)
		v.side = v.xmax - v.xmin + 1;
	else
		v.side = v.ymax - v.ymin + 1;
	out = ft_strnew((v.side + 1) * (v.side) + 1);
	ft_memset(out, '.', (v.side + 1) * (v.side) + 1);
	v.i = v.side;
	while (v.i < (v.side + 1) * v.side)
	{
		out[v.i] = '\n';
		v.i += v.side + 1;
	}
	v.i -= v.side;
	out[v.i] = '\0';
	v.lst_ptr = e->first;
	fillit_fill_output_map(&v, out);
	return (out);
}

void	fillit_print_usage(char *exec_name)
{
	ft_putstr("usage: ");
	ft_putstr(exec_name);
	ft_putendl(" [-c] filename");
}
