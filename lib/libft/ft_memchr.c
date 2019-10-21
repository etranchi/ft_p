/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Etienne <etranchi@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/16 13:01:17 by Etienne           #+#    #+#             */
/*   Updated: 2017/04/18 12:34:17 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char *src;
	unsigned char d;

	src = (unsigned char *)s;
	d = (unsigned char)c;
	while (n--)
	{
		if (*src == d)
			return (src);
		src++;
	}
	return (NULL);
}
