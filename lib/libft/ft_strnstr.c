/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 14:26:24 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 15:18:06 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = (char*)big;
	if (ft_strlen(little) == 0)
		return (str);
	if (ft_strlen(big) == 0 || n < ft_strlen(little))
		return (NULL);
	while (i <= n - ft_strlen(little) && str[i] != '\0')
	{
		if (ft_strncmp(&str[i], little, ft_strlen(little)) == 0)
			return (&str[i]);
		i++;
	}
	return (NULL);
}
