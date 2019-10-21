/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 12:07:34 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 14:35:03 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			len;
	char			*ctn;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	ctn = (char *)malloc((len + 1) * sizeof(char));
	if (!ctn || (*f) == NULL)
		return (NULL);
	ctn[len] = '\0';
	while (s[i])
	{
		ctn[i] = f(i, s[i]);
		i++;
	}
	return (ctn);
}
