/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 12:06:33 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 14:34:46 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	len;
	char	*ctn;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	ctn = (char *)malloc((len + 1) * sizeof(char));
	if (!ctn || !(*f))
		return (NULL);
	ctn[len] = '\0';
	while (s[i])
	{
		ctn[i] = (*f)(s[i]);
		i++;
	}
	return (ctn);
}
