/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit_solve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amulin <amulin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/26 14:40:53 by amulin            #+#    #+#             */
/*   Updated: 2016/02/08 17:47:43 by amulin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

/*
** Inversion de la logique de resolution :
** 0/ Choisir une taille de carre (en partant du plus grand cote parmi tous
**    les tetris)
** 1/ Essayer toutes les combinaisions recursivement
**    1.0/ Si tous les tetris rentrent : sauver le resultat
**    1.1/ Sinon, augmenter le carre de 1 et retour en 1/
*/
