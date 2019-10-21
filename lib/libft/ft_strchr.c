/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 13:41:59 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/30 17:34:37 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char			*str;
	unsigned char	d;
	int				i;

	i = 0;
	str = (char *)s;
	d = (unsigned char)c;
	while (str[i] != '\0')
	{
		if (str[i] == d)
		{
			return (&str[i]);
		}
		i++;
	}
	if (d == '\0')
		return ((char *)(&s[i]));
	else
		return (NULL);
}
