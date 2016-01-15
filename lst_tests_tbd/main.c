#include "libft.h"
#include <stdio.h>

void	lst_start_to_end(t_list *list)
{
	int	i;

	i = 0;
	while (list)
	{
		printf("lst_start_to_end, elem %d, content = %d\n", i, (int)list->content);
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
		printf("lst_end_to_start, elem %d, content = %d\n", i, (int)list->content);
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
	ft_lstappend(&list_start, ft_lstnew(&i, sizeof(int)));
	i++;
	ft_lstappend(&list_start, ft_lstnew(&i, sizeof(int)));

	list_stop = list_start;
	while (list_stop)
	{
		list_stop = list_stop->next;
	}
	printf("Passing lst_start to lst_start_to_end()\n");
	lst_start_to_end(list_start);
	printf("Passing lst_end to lst_end_to_start()\n");
	lst_end_to_start(list_start);
	printf("Final check OK\n");
	return (0);
}
