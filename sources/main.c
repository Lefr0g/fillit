/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:16 by amulin            #+#    #+#             */
/*   Updated: 2016/02/10 16:00:17 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This function is launched only if the input was validated and converted
** by parsing.
** The main job of this function is to launch the recursive solver by calling
** fillit_solve() once, and then to print the resulting map on the
** standard output (uncolorized by default).
*/

int		fillit_run(t_env *e)
{
	t_vars	v;

	fillit_init_vars(&v);
	v.lst_ptr = e->first;


	if (DEBUG_MODE)
	{
		ft_putendl("\033[32mInput map is valid, running rest of the program");
		printf("There are %lu tetriminos\033[0m\n", e->tcount);
	}

	while (v.lst_ptr)
	{
		v.tet_ptr = (v.lst_ptr)->content;
		if (DEBUG_MODE)
			printf("\tSolving now centered on tetri \033[31m%c\033[0m\n",
				v.tet_ptr->letter);
		v.tet_ptr->fixed = 1;
		e->tlocked++;
		if (fillit_solve(e, v.lst_ptr))
			break;
		v.tet_ptr->fixed = 0;
		e->tlocked--;
		v.lst_ptr = v.lst_ptr->next;
	}

	if (DEBUG_MODE)
	{
		ft_putstr("Square size : ");
		ft_putnbr(e->square_size);
		ft_putchar('\n');
	}
//	ft_putchar('\n');
	if (e->color)
		fillit_print_colored(e->result);
	else
		ft_putendl(e->result);
	return (0);
}

/*
** The program supports 2 options :
**  -c for colorized output
**  -s for printing intermediary solutions. Notice that this option greatly
**  slows the program down.
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
			ft_putstr(e->result);
		}
		else
			fillit_error("parsing detected an error in the input", DEBUG_MODE);
	}
	else
		fillit_error("main error exit", DEBUG_MODE);
	fillit_free_all(e);
	if (DEBUG_MODE)
		ft_putendl("\n============ End of program ===========");
	return (0);
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
