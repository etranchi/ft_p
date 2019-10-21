/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 12:10:03 by etranchi          #+#    #+#             */
/*   Updated: 2017/04/19 14:50:13 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		get_nb_words(char *s, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			count++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char		*make_my_string(char *s, char c, int nb)
{
	int		i;
	char	*ctn;
	int		j;
	int		k;

	j = -1;
	i = 0;
	while (++j <= nb)
	{
		k = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			i++;
			k++;
		}
	}
	if (!(ctn = (char *)malloc((i + 1) * sizeof(char))))
		return (NULL);
	j = -1;
	while (++j < k)
		ctn[j] = s[i - k + j];
	ctn[j] = '\0';
	return (ctn);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**tab;
	int		i;
	int		words;
	char	*str;

	if (!s || !c)
		return (NULL);
	str = (char *)s;
	words = get_nb_words(str, c);
	i = 0;
	tab = (char **)malloc((words + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	tab[words] = NULL;
	while (i < words)
	{
		tab[i] = make_my_string(str, c, i);
		i++;
	}
	return (tab);
}
