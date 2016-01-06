/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 16:49:16 by amulin            #+#    #+#             */
/*   Updated: 2016/01/06 18:54:35 by amulin           ###   ########.fr       */
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

/*
** Algo (draft, a optimiser) :
**
**  1/ L'element A est la reference. Il ne bougera pas. L'element B sera place
**     contre lui et deplace progressivement dans le sens des aiguilles d'une
**     montre.
**  2/ On considere le block de A le plus en haut et a gauche. 
**  3/ On ajoute 1 en x afin d'obtenir les coodonnees du premier block de B, 
**     le plus en haut de B.
**  4/ On verifie l'absence de collision entre les 2 tetriminos.
**     4.1/ Si collision, on incremente l'offset de B en x et on revient a 3/.
**  5/ On verifie que les 2 tetriminos sont en contact.
**     5.1/ Si non, on incremente y de 1, on cherche la valeur de x correspondante
**          sur A. On revient ensuite a 3/.
**  6/ Si le positionnement est correct, on compare la taille du carre resultant
**     avec le plus petit trouve jusqu'a present.
**     6.1/ Si le nouveau carre est plus petit, on retient sa valeur, et on
**          enregistre la map resultante sous forme d'une chaine.
**  7/ On continue ce processus jusqu'a ce que B ait fait un tour complet de A.
**  8/ On affiche sur stdout la derniere chaine de caractere enregistree.
**
** Note : Pour ajouter un element C, on fait appel a la recursivite avant 6/
**        en considerant l'assemblage de A et B comme un tetrimino A geant.
**        Puis une fois la boucle de B autour de A finie, on recommence en
**        deplacant cette fois C autour de A.
*/

int		fillit_run(t_env *e)
{
//	int		i;
//	int		x;
//	int		y;
	t_list	*li_ptr;
	t_tetri	*te_ptr;

	ft_putendl("Input map is valid, running rest of the program");

	// Les pointeurs suivants feront reference au second tetri (B)
	li_ptr = e->first->next;
	te_ptr = li_ptr->content;

	// On positionne B par rapport a A
	te_ptr->x_offset = ((t_tetri*)(e->first->content))->x[0] + 1;
	te_ptr->y_offset = ((t_tetri*)(e->first->content))->y[0];
	
	// On boucle jusqu'a ce que tous les tetris aient fini de bouger
//	while (ft_strlen(e->set) < e->tcount)
//	{
		// Check collision
		// Check contact
		// Calculer taille carre
		// Screenshot carre
//	}
	
	return (0);
}

int		fillit_init(t_env **e)
{
	t_tetri		tet_ptr;

	tet_ptr.letter = '\0';
	tet_ptr.x_offset = 0;
	tet_ptr.y_offset = 0;
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
	fillit_free_all(e);
	ft_putendl("============ End of program ===========");
	return (0);
}
