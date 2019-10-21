/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 10:49:31 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 12:28:05 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *cpy;
	t_list *newlst;
	t_list *tmp;

	if (lst == NULL)
		return (NULL);
	cpy = ft_lstnew((*f)(lst)->content, (*f)(lst)->content_size);
	if (cpy == NULL)
		return (NULL);
	lst = lst->next;
	tmp = cpy;
	while (lst)
	{
		newlst = ft_lstnew((*f)(lst)->content, (*f)(lst)->content_size);
		if (!newlst)
			return (NULL);
		lst = lst->next;
		tmp->next = newlst;
		tmp = tmp->next;
	}
	return (cpy);
}
