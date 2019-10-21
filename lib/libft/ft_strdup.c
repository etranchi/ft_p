/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/15 04:47:57 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/18 18:12:00 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
char		*ft_strdup(const char *s1)
{
	char	*pc;
	size_t	i;

	i = ft_strlen(s1);
	if(!s1)
		return (NULL);
	pc = NULL;
	if (!(pc = (char *)malloc((i + 1) * sizeof(char))))
		return (NULL);
	if (!s1)
	{
		pc = NULL;
		return (pc);
	}
	pc[i] = '\0';
	while (i--)
		pc[i] = s1[i];
	return (pc);
}
