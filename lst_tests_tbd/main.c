#include "libft.h"
#include <stdio.h>

void	lst_start_to_end(t_list *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("elem \033[32m%d\033[0m, content = %i\n", i, *(int*)list->content);
		printf("\taddress = \033[036m%p\033[0m\n", list);
		printf("next = %p, prev = %p\n\n", list->next, list->prev);
//		ft_putendl("Mem dump : ");
//		ft_print_memory(list, sizeof(t_list));
//		ft_putchar('\n');
		i++;
		list = list->next;
	}
}

void	lst_end_to_start(t_list *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("lst_end_to_start, elem %d, content = %d\n", i, *(int*)list->content);
		i++;
		list = list->prev;
	}
}

int	main(void)
{
	t_list	*list_start;
	t_list	*list_stop;
	int		i;

	i = 0;
	list_start = ft_lstnew(&i, sizeof(int));
	i++;
	while (i < 5)
	{
		ft_lstappend(&list_start, ft_lstnew(&i, sizeof(int)));
		i++;
	}


	list_stop = list_start;
	while (list_stop->next)
	{
		list_stop = list_stop->next;
	}
	printf("list_start = \033[31m%p\033[0m, list_stop = \033[31m%p\033[0m\n",
			list_start, list_stop);

	printf("\033[32mPassing lst_start to lst_start_to_end()\033[0m\n");
	lst_start_to_end(list_start);
	printf("\n\033[32mPassing lst_end to lst_end_to_start()\033[0m\n");
	lst_end_to_start(list_stop);
	printf("Final check OK\n");
	return (0);
}
