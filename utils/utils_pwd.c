/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 19:51:36 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/30 19:51:38 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void					set_pwd(t_env *e)
{
	char				*pwd;

	pwd = NULL;
	pwd = getwd(pwd);
	e->pwd = ft_strdup(pwd);
	e->curr_pwd = NULL;
	free(pwd);
}

char					*check_ls_cmd(char *cmd)
{
	int					i;
	int					j;
	char				*buff;

	if (!cmd)
		return (NULL);
	if (!(buff = malloc(sizeof(char) * (ft_strlen(cmd) + 1))))
		error_exit("Malloc.\n");
	i = 0;
	j = 0;
	buff[0] = cmd[0];
	while (cmd[++i])
		if (ft_isdigit(cmd[i]))
			continue ;
		else
			buff[++j] = cmd[i];
	buff[++j] = '\0';
	return (buff);
}
