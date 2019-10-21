/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 14:15:39 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/20 10:46:59 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strcat(char *s1, const char *s2)
{
	size_t	i;
	size_t	destlen;

	i = 0;
	destlen = ft_strlen(s1);
	while (s2[i])
	{
		s1[destlen + i] = s2[i];
		i++;
	}
	s1[destlen + i] = '\0';
	return (s1);
}
