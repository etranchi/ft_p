/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/17 14:49:30 by Etienne           #+#    #+#             */
/*   Updated: 2017/10/17 14:49:31 by Etienne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrev(char *str)
{
	char	swap;
	int		b;
	int		c;

	b = 0;
	c = ft_strlen(str) - 1;
	while (b < c)
	{
		swap = str[b];
		str[b] = str[c];
		str[c] = swap;
		b++;
		c--;
	}
	return (str);
}