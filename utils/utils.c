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

void	read_fd_write_fd(t_env *e, int fd_read, int fd_write)
{
	while ((e->len_read = read(fd_read, e->buff, BUFFSIZE)) > 0)
	{
		e->buff[e->len_read] = 0;
		write(fd_write, e->buff, e->len_read);
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < BUFFSIZE)
			break ;
	}
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
