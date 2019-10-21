/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 13:49:45 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/18 18:13:33 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	d;
	char	*str;
	int		i;
	int		count;

	count = 0;
	i = -1;
	str = (char *)s;
	d = (char)c;
	while (str[++i])
	{
		if (str[i] == d)
			count++;
	}
	if (d == '\0')
		return ((char *)(&s[i]));
	while (str && count > 0)
	{
		if (*str == d)
			count--;
		if (count == 0)
			return (str);
		str++;
	}
	return (NULL);
}
