/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/16 18:41:08 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/18 18:44:28 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(long n)
{
	long i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char		*ft_itoa(int n)
{
	char	*ctn;
	int		len;
	int		sign;
	long	nb;

	nb = (long)n;
	sign = nb < 0 ? -1 : 1;
	len = nb < 0 ? get_length(nb) + 1 : get_length(nb);
	nb = nb < 0 ? -nb : nb;
	if (!(ctn = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	ctn[len] = '\0';
	if (sign < 0)
		ctn[0] = '-';
	while (len-- > 0)
	{
		if (sign == -1 && len == 0)
			return (ctn);
		ctn[len] = (nb % 10) + '0';
		nb = nb / 10;
	}
	return (ctn);
}
