/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/28 13:12:51 by etranchi          #+#    #+#             */
/*   Updated: 2017/05/04 12:40:16 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_checkmore(char **more, char *buffer)
{
	char *tmp;

	tmp = NULL;
	if (*more)
	{
		tmp = ft_strdup(*more);
		ft_memdel((void **)more);
		*more = ft_strjoin(tmp, buffer);
		ft_memdel((void **)&tmp);
	}
	else
		*more = ft_strdup(buffer);
}

int			ft_checkbackslash(char **more, char **line)
{
	char *tmp;

	tmp = ft_strchr(*more, '\n');
	if (tmp)
	{
		*line = ft_strsub(*more, 0, ft_strlen(*more) - ft_strlen(tmp));
		ft_memmove(*more, tmp + 1, ft_strlen(tmp));
		return (1);
	}
	return (0);
}

int			ft_read(const int fd, char **more, char **line)
{
	char	buffer[BUFF_SIZE + 1];
	int		ret;

	while ((ret = read(fd, buffer, BUFF_SIZE)))
	{
		if (ret == -1)
			return (-1);
		buffer[ret] = '\0';
		ft_checkmore(more, buffer);
		if (ft_checkbackslash(more, line))
			return (1);
	}
	return (0);
}

int			get_next_line(const int fd, char **line)
{
	int			ret;
	static char *more;

	if (!line || fd < 0)
		return (0);
	if (!more)
		if (!(more = (char *)malloc(sizeof(char) * 1000)))
			return (0);
	ret = ft_read(fd, &more, line);
	if (ret != 0)
		return (ret);
	if (ret == 0)
		return (0);
	if (more == NULL || more[0] == '\0')
		return (0);
	return (1);
}
