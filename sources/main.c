/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*	 Created: 2015/12/28 16:49:16 by amulin			   #+#	  #+#			  */
/*   Updated: 2016/01/14 18:40:50 by amulin           ###   ########.fr       */
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
	ft_putendl("Brut : ");
	e->smallest_size = 4;
	ft_putendl(fillit_save_printable(e));
	e->smallest_size = 0;
	e->inception = 0;
	fillit_move_around(e);
	return (0);
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
	(*e)->tmp->blocks = 0;
	(*e)->tmp->jump = 0;
	(*e)->tmp->layers = 0;
	(*e)->tcount = 0;
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

int		main(int argc, char **argv)
{
	t_env	*e;

	ft_putendl("============ Program Start ============");

	if (fillit_init(&e))
		return (fillit_error("init failed"));
	if (argc == 2)
	{
		if (!fillit_parse(e, argv[1]))
			fillit_run(e);
		else
			fillit_error("parsing detected an error in the input");
	}
	else
		fillit_error("main error exit");


	fillit_print_raw(e);
	printf("There are %lu tetriminos\n", e->tcount);	
	ft_putstr("Square size : ");
	ft_putnbr(e->smallest_size);
	ft_putchar('\n');
	ft_putendl("============ Betatest print ===========\n");
	ft_putendl(e->result);
	fillit_free_all(e);
	ft_putendl("\n============ End of program ===========");
	return (0);
}
