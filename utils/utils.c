/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 19:11:07 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/27 19:11:08 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

int		get_status_fd(int fd, int out)
{
	char *status;

	status = NULL;
	while (get_next_line(fd, &status) > 0)
	{
		if (!ft_strncmp("ERROR", status, 5))
		{
			if (out == STDOUT)
				put_msg_on_fd(STDOUT, status, 0);
			free(status);
			return (0);
		}
		if (!ft_strncmp("SUCCESS", status, 5))
		{
			if (out == STDOUT && ft_strlen(status) > 9)
				put_msg_on_fd(STDOUT, status, 0);
			free(status);
			return (1);
		}
		free(status);
	}
	return (0);
}

void	error_exit(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}

void	check_free(char const *s1, char const *s2, int f1, int f2)
{
	if (f1)
		free((void *)s1);
	if (f2)
		free((void *)s2);
}

char	*ft_strjoin_free(char const *s1, char const *s2, int f1, int f2)
{
	size_t	len;
	char	*ctn;
	int		i;
	int		j;

	if (!s1 && s2)
		return ((char *)s2);
	if (!s2 && s1)
		return ((char *)s1);
	if (!s1 && !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	ctn = (char *)malloc((len + 1) * sizeof(char));
	if (!ctn)
		return (NULL);
	ctn[len] = '\0';
	i = -1;
	while (s1[++i])
		ctn[i] = s1[i];
	j = -1;
	while (s2[++j])
		ctn[i + j] = s2[j];
	check_free(s1, s2, f1, f2);
	return (ctn);
}

void	error(t_env *e, char *reason, char **tab)
{
	ft_putstr("ERROR | ");
	ft_putstr(reason);
	ft_putstr("\n");
	(void)e;
	free_tab(tab);
}
