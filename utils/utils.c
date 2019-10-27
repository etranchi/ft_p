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

void	merge_data(t_data *data, char *buff, int len_read)
{
	char *tmp;

	tmp = NULL;
	if(!data->data)
	{
		data->data = ft_memalloc(len_read);
		ft_memcpy((void*)data->data, buff, len_read);
	}
	else
	{
		tmp = data->data;
		data->data = ft_memalloc(data->size + len_read + 1);
		ft_memcpy((void*)data->data, tmp, data->size);
		ft_memcpy((void*)&data->data[data->size], buff, len_read);
	}
	data->size += len_read;
}

void	read_fd_write_fd(t_env *e, int fd_read, int fd_write)
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data));
	ft_memset(data, 0, sizeof(t_data));
	while ((e->len_read = read(fd_read, e->buff, BUFFSIZE)) > 0)
	{

		e->buff[e->len_read] = 0;
		merge_data(data, e->buff, e->len_read);
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < BUFFSIZE)
		{
			write(fd_write, data->data, data->size);
			break ;
		}
			
	}
}

t_data	*read_fd(t_env *e, int fd)
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data));
	ft_memset(data, 0, sizeof(t_data));
	while ((e->len_read = read(fd, e->buff, BUFFSIZE)))
	{
		e->buff[e->len_read] = '\0';
		merge_data(data, e->buff, e->len_read);
		if (e->len_read < BUFFSIZE)
			return data;
		
	}
	return data;
}

void	error_exit(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}

void	put_msg_on_fd(int fd, char *msg)
{
	char				*to_send;

	to_send = ft_strjoin(msg, "\n");
	write(fd, to_send, ft_strlen(to_send));
}
