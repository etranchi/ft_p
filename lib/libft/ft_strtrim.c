/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 13:23:49 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 15:03:37 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_start(char const *str)
{
	size_t i;

	i = 0;
	while (str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
		i++;
	return (i);
}

static size_t	ft_end(char const *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	while (str[i] == '\0' || str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
		i--;
	return (i);
}

char			*ft_strtrim(char const *s)
{
	size_t	start;
	size_t	end;
	char	*ctn;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	start = ft_start(s);
	if (s[start] == '\0')
		return (ft_strdup(""));
	end = ft_end(s);
	if (start > end)
		return (ft_strdup(""));
	ctn = (char *)malloc((end - start + 2) * sizeof(char));
	if (!ctn)
		return (NULL);
	while (i < (end - start + 1))
	{
		ctn[i] = s[start + i];
		i++;
	}
	ctn[i] = '\0';
	return (ctn);
}
