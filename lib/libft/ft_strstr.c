/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 14:41:43 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/18 18:20:59 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*str;

	j = 0;
	i = -1;
	str = (char *)s1;
	if (str[j] == '\0' && s2[j] != '\0')
		return (NULL);
	if ((str[j] == '\0' && s2[j] == '\0') || s2[j] == '\0')
		return (str);
	while (str[++i])
	{
		if (s2[j] == str[i])
		{
			while (str[i + j] == s2[j] && j < ft_strlen(s2))
			{
				if (j == ft_strlen(s2) - 1)
					return (&str[i]);
				j++;
			}
		}
		j = 0;
	}
	return (0);
}
