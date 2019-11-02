/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_dir_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 16:52:13 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/31 16:52:14 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void						perform_mkdir(t_env *e)
{
	char					**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1] && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | No dir to create.", 0));
	if (access(tab_cmd[1], F_OK) != -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Name already used.", 0));
	if (mkdir(tab_cmd[1], ACCESSPERMS) == -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | mkdir failed.", 0));
	put_msg_on_fd(e->c_sock,
		ft_strjoin("SUCCESS | Created dir ", tab_cmd[1]), 1);
	free_tab(tab_cmd);
}

void						perform_rmdir(t_env *e)
{
	char					**tab_cmd;
	int						fd;
	struct stat				file_stat;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1] && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | No dir to rm.", 0));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Dir doesn't exist.", 0));
	if (fstat(fd, &file_stat) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Fstat error.", 0));
	if (!S_ISDIR(file_stat.st_mode) && free_tab(tab_cmd))
	{
		return (put_msg_on_fd(e->c_sock,
			"ERROR | This is not a directory.", 0));
	}
	if (rmdir(tab_cmd[1]) != 0 && free_tab(tab_cmd))
	{
		return (put_msg_on_fd(e->c_sock,
			"ERROR | Rmdir failed. Dir maybe not empty.", 0));
	}
	put_msg_on_fd(e->c_sock,
		ft_strjoin("SUCCESS | Rmdir ", tab_cmd[1]), 1);
	free_tab(tab_cmd);
}

void						perform_unlink(t_env *e)
{
	char					**tab_cmd;
	int						fd;
	struct stat				file_stat;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1] && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to unlink.", 0));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | File doesn't exist.", 0));
	if (fstat(fd, &file_stat) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Fstat error.", 0));
	if (!S_ISREG(file_stat.st_mode) && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | This is not a file.", 0));
	if (unlink(tab_cmd[1]) != 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Unlink failed.", 0));
	put_msg_on_fd(e->c_sock,
		ft_strjoin("SUCCESS | Unlink ", tab_cmd[1]), 1);
	free_tab(tab_cmd);
}
