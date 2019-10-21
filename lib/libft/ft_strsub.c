/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 12:20:04 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 15:20:52 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strsub(char const *s, unsigned int start, size_t size)
{
	unsigned int	len;
	char			*ctn;
	unsigned int	i;
	unsigned int	j;

	if (!s)
		return (NULL);
	j = (unsigned int)size;
	len = ft_strlen(s);
	if (start > len || start + size > len)
		return (NULL);
	ctn = (char *)malloc((j + 1) * sizeof(char));
	if (!ctn)
		return (NULL);
	i = 0;
	while (i < j && s[start] != '\0')
	{
		ctn[i] = s[start + i];
		i++;
	}
	ctn[i] = '\0';
	return (ctn);
}
