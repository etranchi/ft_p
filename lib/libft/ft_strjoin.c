/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 12:06:08 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 14:49:42 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*ctn;
	int		i;
	int		j;

	if (!s1 && s2)
		return ((char *)s2);
	if (!s2 && s1)
		return ((char *)s1);
	if (!s1 && !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	ctn = (char *)malloc((len + 1) * sizeof(char));
	if (!ctn)
		return (NULL);
	ctn[len] = '\0';
	i = -1;
	while (s1[++i])
		ctn[i] = s1[i];
	j = -1;
	while (s2[++j])
		ctn[i + j] = s2[j];
	return (ctn);
}
