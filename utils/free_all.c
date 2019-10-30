/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 12:32:40 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/28 12:32:41 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

int		free_tab(char **tab)
{
	int i;

	i = -1;
	if (tab)
	{
		while (tab[++i])
			free(tab[i]);
		free(tab);
	}
	return (1);
}

void	free_data(t_data *data)
{
	if (data)
	{
		if (data->data && data->size > 0)
		{
			free(data->data);
			data->data = NULL;
			free(data);
		}
		
	}
}
