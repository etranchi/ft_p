/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_find.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <etranchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/25 14:14:36 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 13:06:33 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_lst	*ft_list_find(t_lst *begin_list, void *data_ref, int (*cmp)())
{
	t_lst *tmp;

	tmp = begin_list;
	while (tmp != NULL)
	{
		if ((*cmp)(data_ref, tmp->data) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
