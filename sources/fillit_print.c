/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 19:54:08 by amulin            #+#    #+#             */
/*   Updated: 2015/12/28 20:25:18 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

void	fillit_print_raw(t_env *e)
{
	int		i;
	t_list	*ptr;
	t_tetri	*tetptr;

	ptr = e->first;
	i = 0;
	ft_putendl("\n================ DEBUG ================");
	while (ptr)
	{
		tetptr = (t_tetri*)ptr->content;
		ft_putstr("Tetrimino ");
		ft_putnbr(i);
		ft_putstr(" contains : ");
		ft_putendl(tetptr->raw);
		ptr = ptr->next;
		i++;
	}
}
