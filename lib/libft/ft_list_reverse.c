/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_reverse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <etranchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 13:08:08 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 13:08:04 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_reverse(t_lst **begin_list)
{
	t_lst *next;
	t_lst *current;
	t_lst *last;

	last = NULL;
	current = *begin_list;
	while (current != NULL)
	{
		next = current->next;
		current->next = last;
		last = current;
		current = next;
	}
	*begin_list = last;
}
