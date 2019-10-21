/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_push_back.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <etranchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 18:35:23 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 13:07:18 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_push_back(t_lst **begin_list, void *data)
{
	t_lst *elm;
	t_lst *temp;

	elm = ft_create_elem(data);
	temp = *begin_list;
	if (*begin_list == NULL)
		*begin_list = elm;
	else
	{
		while (temp && temp->next)
			temp = temp->next;
		temp->next = elm;
	}
}
