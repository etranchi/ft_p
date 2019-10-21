/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 12:38:33 by Etienne           #+#    #+#             */
/*   Updated: 2017/04/17 16:14:10 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

void	ft_bzero(void *s, size_t n)
{
	char			*str;
	unsigned int	i;

	i = (unsigned int)n;
	str = (char *)s;
	while (i--)
	{
		str[i] = 0;
	}
}
