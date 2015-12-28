
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
	ft_putendl("Input map is valid, running resolution program");
	return (0);
}

int		fillit_init(t_env **e)
{
	t_tetri		elem;

	elem.letter = '\0';
	elem.raw = NULL;

	(*e) = (t_env*)malloc(sizeof(t_env));
	if (!*e)
		return (-1);
	(*e)->tmp = (t_tmp*)malloc(sizeof(t_tmp));
	if (!(*e)->tmp)
		return (-1);
	(*e)->tmp->line = ft_strnew(10);
	if (!(*e)->tmp->line)
		return (-1);
	(*e)->first = ft_lstnew((void*)&elem, sizeof(t_tetri));
	if (!(*e)->first)
		return (-1);
	return (0);
}

void	fillit_free_all(t_env *e)
{
	ft_strdel(&(e->tmp->line));
	free(e->tmp);
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
			fillit_run();
		else
			fillit_error("parse failed");
	}
	else
		fillit_error("main error exit");
	fillit_free_all(e);
	ft_putendl("============ End of program ===========");
	return (0);
}
