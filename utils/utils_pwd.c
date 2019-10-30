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
