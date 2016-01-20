/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*	 Created: 2015/12/28 16:49:16 by amulin			   #+#	  #+#			  */
/*   Updated: 2016/01/20 15:03:11 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "fillit.h"

int		fillit_error(char *str)
{
	if (!str)
		ft_putstr_fd("error", 2);
	else
	{
		ft_putstr_fd("error : ", 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n", 2);
	return (-1);
}

/*
 * Je vire tout, mais c'est tjrs dans l'historique git
*/

int		fillit_run(t_env *e)
{

	ft_putendl("Input map is valid, running rest of the program");
	printf("There are %lu tetriminos\n", e->tcount);	
	e->inception = 0;
	fillit_move_around(e);
	return (0);
}

void	fillit_reset_tmp(t_tmp *tmp)
{
	tmp->fd = 0;
	tmp->i = 0;
	tmp->isaved = -1;
	tmp->xmax = 0;
	tmp->ymax = 0;
	tmp->height = -1;
	tmp->pos = 0;
	tmp->gnl_ret = 0;
	tmp->jump = 0;
	tmp->layers = 0;
	tmp->layercheck_ret = 0;
	tmp->blocks = 0;
	ft_bzero(tmp->line, sizeof(tmp->line));
}

int		fillit_init(t_env **e)
{
	t_tetri		tet_ptr;

	tet_ptr.letter = '\0';
	tet_ptr.x_offset = 0;
	tet_ptr.y_offset = 0;
	tet_ptr.fixed = 1;
	ft_bzero(tet_ptr.raw, 17);

	if (!(*e = (t_env*)malloc(sizeof(t_env))))
		return (-1);
	if (!((*e)->tmp = (t_tmp*)malloc(sizeof(t_tmp))))
		return (-1);
	if (!((*e)->tmp->line = ft_strnew(10)))
		return (-1);
	fillit_reset_tmp((*e)->tmp);
	(*e)->tcount = 0;
	(*e)->update = 0;
	(*e)->color = 0;
	(*e)->smallest_size = 0;
	ft_bzero((*e)->set, 25);
	if (!((*e)->first = ft_lstnew(&tet_ptr, sizeof(t_tetri))))
		return (-1);
	return (0);
}

void	fillit_reset_quickvars(t_env *e)
{
	e->block = 0;
	e->i = 0;
	e->j = 0;
	e->x = 0;
	e->y = 0;
}

void	fillit_del_tetri(void *content, size_t size)
{
	ft_bzero(content, size);
	free((t_tetri*)content);
}


void	fillit_free_all(t_env *e)
{
	ft_strdel(&(e->tmp->line));
	free(e->tmp);
	ft_lstdel(&e->first, &fillit_del_tetri);
	free(e);
}

int		main(int ac, char **av)
{
	t_env	*e;

	ft_putendl("============ Program Start ============");

	if (fillit_init(&e))
		return (fillit_error("init failed"));
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 's'))
		e->update = 1;
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 'c'))
		e->color = 1;

	if (ac >= 2)
	{

		if (!fillit_parse(e, av[ac - 1]))
		{
			fillit_run(e);
			printf("There are %lu tetriminos\n", e->tcount);
			ft_putstr("Square size : ");
			ft_putnbr(e->smallest_size);
			ft_putchar('\n');
			if (e->color)
				fillit_print_colored(e->result);
			else
				ft_putendl(e->result);
		}
		else
			fillit_error("parsing detected an error in the input");
	}
	else
		fillit_error("main error exit");

	fillit_free_all(e);
	ft_putendl("\n============ End of program ===========");
	return (0);
}
