/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_elem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 18:17:08 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 13:08:35 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_lst	*ft_create_elem(void *data)
{
	t_lst *elm;

	elm = malloc(sizeof(t_lst));
	if (elm == NULL)
		return (elm);
	elm->data = data;
	elm->next = NULL;
	return (elm);
}
