/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_remove_if.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <etranchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 14:20:17 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 13:07:45 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_remove_if(t_lst **begin_list, void *data_ref, int (*cmp)())
{
	t_lst *tmp;
	t_lst *tmp1;

	while (*begin_list != NULL && (*cmp)(data_ref, (*begin_list)->data) == 0)
	{
		tmp = *begin_list;
		*begin_list = (*begin_list)->next;
		free(tmp);
	}
	if (*begin_list == NULL)
		return ;
	tmp = *begin_list;
	tmp1 = (*begin_list)->next;
	while (tmp1 != NULL)
	{
		if ((*cmp)(data_ref, tmp->data) == 0)
		{
			tmp->next = tmp1->next;
			free(tmp1);
			tmp1 = tmp;
		}
		tmp = tmp1;
		tmp1 = tmp1->next;
	}
}
