/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 19:46:48 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/30 19:46:50 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void					perform_get(t_env *e)
{
	int					fd;
	char				**tab_cmd;
	struct stat			file_stat;
	char				*to_send;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1] && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to get.", 0));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Can't open file.", 0));
	if (fd == -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | File doesn't exist.", 0));
	if (fstat(fd, &file_stat) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Fstat.", 0));
	if (!S_ISREG(file_stat.st_mode) && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | It's not a file.", 0));
	free_tab(tab_cmd);
	put_msg_on_fd(e->c_sock, "SUCCESS", 0);
	to_send = ft_strjoin(e->cmd, " ");
	to_send = ft_strjoin_free(to_send, ft_itoa(file_stat.st_mode), 1, 1);
	nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
	put_msg_on_fd(e->c_sock, to_send, 1);
	if (!get_status_fd(e->c_sock, -1))
		return ;
	read_fd_write_fd(e, fd, e->c_sock);
}

void					perform_put(t_env *e)
{
	char				**tab_cmd;
	int					fd;
	char				*to_send;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1] && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to put.", 0));
	if (access(tab_cmd[1], F_OK) != -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | File already exist.", 0));
	if ((fd = open(tab_cmd[1], O_RDONLY | O_CREAT | O_WRONLY,
		ft_atoi(tab_cmd[2]))) < 0 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | Can't create file.", 0));
	to_send = ft_strjoin("SUCCESS | Created ", tab_cmd[1]);
	free_tab(tab_cmd);
	put_msg_on_fd(e->c_sock, to_send, 1);
	nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
	read_fd_write_fd(e, e->c_sock, fd);
}

void					perform_pwd(t_env *e)
{
	put_msg_on_fd(e->c_sock, "SUCCESS | pwd", 0);
	nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
	put_msg_on_fd(e->c_sock, e->curr_pwd ? e->curr_pwd : e->pwd, 0);
}

void					perform_cd(t_env *e)
{
	char				**tab_cmd;
	char				*tmp_pwd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (chdir(tab_cmd[1] ? tab_cmd[1] : ".") == -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(e->c_sock, "ERROR | cd", 0));
	tmp_pwd = getwd(NULL);
	if (ft_strlen(tmp_pwd) < ft_strlen(e->pwd) && chdir(e->pwd) != -1)
		put_msg_on_fd(e->c_sock,
			"ERROR : You don't have the rights to do this.", 0);
	else
	{
		if (e->curr_pwd)
			free(e->curr_pwd);
		if (!tab_cmd[1])
			e->curr_pwd = ft_strdup(e->pwd);
		else
			e->curr_pwd = ft_strdup(tmp_pwd);
		put_msg_on_fd(e->c_sock,
			ft_strjoin_free("SUCCESS | cd | ", e->curr_pwd, 0, 0), 1);
		chdir(e->curr_pwd);
	}
	free(tmp_pwd);
	free_tab(tab_cmd);
}

void					perform_ls(t_env *e)
{
	pid_t				pid;
	char				**cmd;

	pid = fork();
	if (pid == -1)
		error_exit("Fork.\n");
	if (pid == 0)
	{
		put_msg_on_fd(e->c_sock, "SUCCESS | ls", 0);
		cmd = ft_strsplit(e->cmd, ' ');
		if (cmd[1] && cmd[1][0] != '-')
			cmd[1] = ".";
		if (cmd[2])
			cmd[2] = NULL;
		dup2(e->c_sock, 1);
		dup2(e->c_sock, 2);
		execv("/bin/ls", cmd);
		free_tab(cmd);
		exit(0);
	}
	wait4(pid, 0, 0, NULL);
	put_msg_on_fd(e->c_sock, "NTD", 0);
}
