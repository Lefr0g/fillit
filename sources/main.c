/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sources/main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:16 by amulin            #+#    #+#             */
/*   Updated: 2016/01/21 18:49:31 by liumsn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** This funtion printfs a standard or specified error message on the error
** output.
** -------------------------- To be added to libft ----------------------------
*/

int		fillit_error(char *str)
{
	ft_putendl("error");
	ft_putstr_fd("error : ", 2);
	ft_putendl_fd(str, 2);
	return (-1);
}

/*
** This function is launched only if the input was validated and converted
** by parsing.
** e->inception gives the "deepness" of recursion, it was used mainly
** for debug purposes.
** The main job of this function is to launch the recursive solver by calling
** fillit_move_around() once, and then to print the resulting map on the
** standard output (uncolorized by default).
*/

int		fillit_run(t_env *e)
{
	e->inception = 0;
	fillit_move_around(e);
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

	if (fillit_init(&e))
		return (fillit_error("init failed"));
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 's'))
		e->update = 1;
	if (ac > 2 && av[1][0] == '-' && ft_strchr(av[1], 'c'))
		e->color = 1;
	if (ac >= 2)
	{
		if (!fillit_parse(e, av[ac - 1]))
			fillit_run(e);
		else
			fillit_error("parsing detected an error in the input");
	}
	else
		fillit_error("main error exit");
	fillit_free_all(e);
	return (0);
}
