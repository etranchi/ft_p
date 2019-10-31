/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_dir_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 16:42:55 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/31 16:42:56 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void		send_mkdir(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
}

void		send_rmdir(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
}
