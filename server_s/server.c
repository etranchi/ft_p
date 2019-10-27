/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 15:22:52 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/21 15:22:54 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void					create_server(t_env *e)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;
	unsigned int		cslen;
	struct sockaddr_in	csin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Binding.\n");
	if (listen(e->sock, 42) == -1)
		error_exit("Listen.\n");
	if ((e->c_sock = accept(e->sock, (struct sockaddr *)&csin, &cslen)) == -1)
		error_exit("Accept.\n");
}

void					perform_get(t_env *e)
{
	int					fd;
	char				**tab_cmd;
	struct stat			file_stat;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to put."));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return (put_msg_on_fd(e->c_sock, ft_strjoin("ERROR | Can't open :", tab_cmd[1])));
	if (fd == -1)
		return (put_msg_on_fd(e->c_sock, "ERROR | File doesn't exist."));
	if (fstat(fd, &file_stat) < 0)
		return (put_msg_on_fd(e->c_sock, "ERROR | Fstat.\n"));
	if (!S_ISREG(file_stat.st_mode))
		return (put_msg_on_fd(e->c_sock, "ERROR | It's not a file."));
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, ft_itoa(file_stat.st_mode));
	put_msg_on_fd(e->c_sock, e->cmd);
	read_fd_write_fd(e, fd, e->c_sock);
}

void					perform_put(t_env *e)
{
	char				**cmd;
	int					i;
	int					fd;

	i = -1;
	cmd = ft_strsplit(e->cmd, ' ');
	if (!cmd[1])
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to put."));
	if (access(cmd[1], F_OK) != -1)
		return (put_msg_on_fd(e->c_sock, "ERROR | File already exist."));
	if ((fd = open(cmd[1], O_RDONLY | O_CREAT | O_WRONLY, ft_atoi(cmd[2]))) < 0)
		return (put_msg_on_fd(e->c_sock, "ERROR | Can't create file."));
	e->cmd = ft_strjoin("SUCCESS | Created ", cmd[1]);
	put_msg_on_fd(e->c_sock, e->cmd);
	read_fd_write_fd(e, e->c_sock, fd);
	e->len_read = 0;
}

void					perform_pwd(t_env *e)
{
	char				*to_send;

	to_send = ft_strjoin("SUCCESS | pwd | ", e->curr_pwd);
	put_msg_on_fd(e->c_sock, to_send);
}

void					perform_quit(t_env *e)
{
	char				*to_send;

	to_send = ft_strdup("SUCCESS | quit");
	put_msg_on_fd(e->c_sock, to_send);
}

void					perform_cd(t_env *e, char **cmd)
{
	char				*to_send;
	char				*save_pwd;

	if (chdir(cmd[1] ? cmd[1] : ".") == -1)
	{
		to_send = ft_strjoin("ERROR | ", e->cmd);
		return (put_msg_on_fd(e->c_sock, to_send));
	}
	save_pwd = e->curr_pwd;
	e->curr_pwd = getcwd(NULL, 0);
	if (ft_strlen(e->curr_pwd) < ft_strlen(e->pwd))
	{
		to_send = ft_strdup("ERROR : You don't have the rights to do this.");
		put_msg_on_fd(e->c_sock, to_send);
		chdir(save_pwd);
		e->curr_pwd = save_pwd;
	}
	else
	{
		if (!cmd[1])
			e->curr_pwd = e->pwd;
		to_send = ft_strjoin("SUCCESS | cd | ", e->curr_pwd);
		put_msg_on_fd(e->c_sock, to_send);
		chdir(e->curr_pwd);
	}
}

void					perform_cmd(t_env *e)
{
	char				**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strncmp(tab_cmd[0], "ls", 3))
		perform_ls(e);
	else if (!ft_strncmp(tab_cmd[0], "cd", 3))
		perform_cd(e, tab_cmd);
	else if (!ft_strncmp(tab_cmd[0], "get", 4))
		perform_get(e);
	else if (!ft_strncmp(tab_cmd[0], "put", 4))
		perform_put(e);
	else if (!ft_strncmp(tab_cmd[0], "pwd", 4))
		perform_pwd(e);
	else if (!ft_strncmp(tab_cmd[0], "quit", 5))
		perform_quit(e);
	else
	{
		e->cmd = ft_strjoin(e->cmd, " not found.");
		e->cmd = ft_strjoin("ERROR : ", e->cmd);
		put_msg_on_fd(e->c_sock, e->cmd);
	}
}

void					set_pwd(t_env *e)
{
	char				*pwd;

	pwd = NULL;
	pwd = getwd(pwd);
	e->pwd = ft_strdup(pwd);
	e->curr_pwd = ft_strdup(e->pwd);
}

char					*check_ls_cmd(char *cmd)
{
	int					i;
	int					j;
	char				*buff;

	if (!cmd)
		return (NULL);
	if (!(buff = malloc(sizeof(char) * ft_strlen(cmd))))
		error_exit("Malloc.\n");
	i = -1;
	j = -1;
	while (cmd[++i])
		if (ft_isdigit(cmd[i]))
			continue ;
		else
			buff[++j] = cmd[i];
	buff[++j] = '\0';
	return (buff);
}

void					perform_ls(t_env *e)
{
	pid_t				pid;
	char				**cmd;
	char				*to_send;

	pid = fork();
	if (pid == -1)
		error_exit("Fork.\n");
	if (pid == 0)
	{
		to_send = ft_strdup("SUCCESS | ls");
		put_msg_on_fd(e->c_sock, to_send);
		cmd = ft_strsplit(e->cmd, ' ');
		dup2(e->c_sock, 1);
		dup2(e->c_sock, 2);
		cmd[0] = ft_strjoin("/bin/", cmd[0]);
		if (cmd[1] && cmd[1][0] != '-')
			cmd[1] = NULL;
		cmd[1] = check_ls_cmd(cmd[1]);
		if (cmd[2])
			cmd[2] = NULL;
		execv(cmd[0], cmd);
		close(e->c_sock);
	}
	wait4(pid, 0, 0, NULL);
}

int						main(int ac, char **av)
{
	t_env				*e;

	if (ac < 2)
		error_exit("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
	e->port = ft_atoi(av[1]);
	create_server(e);
	while ((get_next_line(e->c_sock, &e->cmd)) > 0)
	{
		if (ft_strlen(e->cmd) > 0)
			perform_cmd(e);
		ft_bzero(e->cmd, ft_strlen(e->cmd));
		e->error = 0;
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
