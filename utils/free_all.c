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

void	free_tab(char **tab)
{
	int i;

	i = -1;
	if (tab)
	{
		while (tab[++i])
		{
			printf("free_tab tab[i] : %p\n", tab[i]);
			free(tab[i]);
		}
		printf("free tab : %p\n", tab);
		free(tab);
	}
}

void	free_data(t_data *data)
{
	if (data)
	{
		if (data->data && data->size > 0)
		{
			printf("free_data data->data : %p\n", data->data);
			free(data->data);
			data->data = NULL;
			free(data);
		}
		
	}
}
