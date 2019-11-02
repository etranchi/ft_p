/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 19:38:25 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/30 19:38:26 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void						put_file(t_env *e)
{
	int						fd;
	char					**tab_cmd;
	struct stat				file_stat;
	char					*to_send;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to put.", tab_cmd));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return (error(e, "File doesn't exist.", tab_cmd));
	if (fstat(fd, &file_stat) < 0)
		return (error(e, "Fstat.\n", tab_cmd));
	if (!S_ISREG(file_stat.st_mode))
		return (error(e, "It's not a file.", tab_cmd));
	free_tab(tab_cmd);
	to_send = ft_strjoin(e->cmd, " ");
	to_send = ft_strjoin_free(to_send, ft_itoa(file_stat.st_mode), 1, 1);
	put_msg_on_fd(e->sock, to_send, 1);
	if (!get_status_fd(e->sock, STDOUT))
		return ;
	read_fd_write_fd(e, fd, e->sock);
}

void						get_file(t_env *e)
{
	char					**tab_cmd;
	char					**file_info;
	int						fd;
	t_data					*data;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to get.", tab_cmd));
	if (access(tab_cmd[1], F_OK) != -1)
		return (error(e, "File already exist.", tab_cmd));
	put_msg_on_fd(e->sock, e->cmd, 0);
	free_tab(tab_cmd);
	if (!get_status_fd(e->sock, STDOUT))
		return ;
	if ((data = read_fd(e, e->sock)) == NULL)
		return ;
	file_info = ft_strsplit(data->data, ' ');
	if ((fd = open(file_info[1], O_RDONLY | O_CREAT | O_WRONLY,
		ft_atoi(file_info[2]))) < 0)
		return (error(e, "Can't create file.", file_info));
	put_msg_on_fd(e->sock, "SUCCESS", 0);
	read_fd_write_fd(e, e->sock, fd);
	printf("SUCCESS | Created %s\n", file_info[1]);
	free_tab(file_info);
	free_data(data);
}

void						get_cd(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
}

void						get_pwd(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
	read_fd_write_fd(e, e->sock, STDOUT);
}

void						get_ls(t_env *e)
{
	t_data *data;

	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
	while ((data = read_fd(e, e->sock)))
	{
		if (ft_strstr(data->data, "NTD"))
		{
			free_data(data);
			return ;
		}
		else
			write_data_on_fd(e, data, STDOUT);
	}
}
