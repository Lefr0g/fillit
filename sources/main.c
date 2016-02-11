/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:16 by amulin            #+#    #+#             */
/*   Updated: 2016/02/11 19:39:20 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
**  -c for colorized output
** The logic below is quite self-explanatory.
*/

int		main(int ac, char **av)
{
	t_env	*e;

	if (DEBUG_MODE)
		ft_putendl("============ Program Start ============");
	if (fillit_init(&e))
		return (fillit_error("init failed", DEBUG_MODE));
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 's'))
		e->update = 1;
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 'c'))
		e->color = 1;
	if (ac >= 2)
		fillit_proceed(e, ac, av);
	else
		fillit_error("main error exit", DEBUG_MODE);
	fillit_free_all(e);
	if (DEBUG_MODE)
		ft_putendl("\n============ End of program ===========");
	return (0);
}

/*
** This is the core of the program, put into a subfunction for norme compliance
*/

void	fillit_proceed(t_env *e, int ac, char **av)
{
	if (!fillit_parse(e, av[ac - 1]))
	{
		e->square_size = 2;
		while (!e->result)
		{
			fillit_solve(e, e->first);
			if (!e->result)
				e->square_size++;
		}
		(e->color) ? fillit_colorprint(e->result) : ft_putstr(e->result);
	}
	else
		fillit_error("parsing detected an error in the input", DEBUG_MODE);
}

/*
** Final free before program exit.
*/

void	fillit_free_all(t_env *e)
{
	ft_strdel(&(e->tmp->line));
	free(e->tmp);
	ft_lstdel(&e->first, &fillit_del_tetri);
	free(e);
}
