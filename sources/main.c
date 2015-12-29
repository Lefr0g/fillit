/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:16 by amulin            #+#    #+#             */
/*   Updated: 2015/12/29 14:54:49 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

int 	fillit_error(char *str)
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

int		fillit_run(void)
{
	ft_putendl("Input map is valid, running rest of the program");
	return (0);
}

int		fillit_init(t_env **e)
{
	t_tetri		*elem;

	if (!(elem = (t_tetri*)malloc(sizeof(t_tetri))))
		return (-1);
	elem->letter = '\0';
	ft_bzero(elem->raw, 17);

	if (!(*e = (t_env*)malloc(sizeof(t_env))))
		return (-1);
	if (!((*e)->tmp = (t_tmp*)malloc(sizeof(t_tmp))))
		return (-1);
	if (!((*e)->tmp->line = ft_strnew(10)))
		return (-1);
	if (!((*e)->first = ft_lstnew((void*)elem, sizeof(t_tetri))))
		return (-1);
	return (0);
}

void	fillit_free_all(t_env *e)
{
	ft_strdel(&(e->tmp->line));
	free(e->tmp);
	free(e);
//	TODO : free list;
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
			fillit_run();
		else
			fillit_error("parsing detected an error in the input");
	}
	else
		fillit_error("main error exit");

	fillit_print_raw(e);
	
	fillit_free_all(e);
	ft_putendl("============ End of program ===========");
	return (0);
}
