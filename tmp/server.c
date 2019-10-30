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



// PAS TOUCHE PUT
void					perform_put(t_env *e)
{
	char				**tab_cmd;
	int					fd;
	char				*to_send;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (put_msg_on_fd(e->c_sock, "ERROR | No file to put.", 0));
	if (access(tab_cmd[1], F_OK) != -1)
		return (put_msg_on_fd(e->c_sock, "ERROR | File already exist.", 0));
	if ((fd = open(tab_cmd[1], O_RDONLY | O_CREAT | O_WRONLY, ft_atoi(tab_cmd[2]))) < 0)
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
	put_msg_on_fd(e->c_sock, e->curr_pwd, 0);
}

void					perform_quit(t_env *e)
{
	put_msg_on_fd(e->c_sock, "SUCCESS | quit", 0);
}

void					perform_cd(t_env *e)
{
	char				*to_send;
	char				**tab_cmd;
	char 				*tmp_pwd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (chdir(tab_cmd[1] ? tab_cmd[1] : ".") == -1)
	{
		to_send = ft_strjoin("ERROR | ", e->cmd);
		free_tab(tab_cmd);
		return (put_msg_on_fd(e->c_sock, to_send, 1));
	}
	tmp_pwd = getwd(NULL);
	if (ft_strlen(tmp_pwd) < ft_strlen(e->pwd))
	{
		to_send = ft_strdup("ERROR : You don't have the rights to do this.");
		put_msg_on_fd(e->c_sock, to_send, 1);
		chdir(e->curr_pwd);
	}
	else
	{
		if (e->curr_pwd)
			free(e->curr_pwd);
		if (!tab_cmd[1])
			e->curr_pwd = ft_strdup(e->pwd);
		else
			e->curr_pwd = ft_strdup(tmp_pwd);
		to_send = ft_strjoin_free("SUCCESS | cd | ", e->curr_pwd, 0 , 0);
		put_msg_on_fd(e->c_sock, to_send, 1);
		chdir(e->curr_pwd);
	}
	free(tmp_pwd);
	free_tab(tab_cmd);
}

void					perform_cmd(t_env *e)
{
	char				**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strncmp(tab_cmd[0], "ls", 2))
		perform_ls(e);
	else if (!ft_strncmp(tab_cmd[0], "cd", 2))
		perform_cd(e);
	else if (!ft_strncmp(tab_cmd[0], "get", 3))
		perform_get(e);
	else if (!ft_strncmp(tab_cmd[0], "put", 3))
		perform_put(e);
	else if (!ft_strncmp(tab_cmd[0], "pwd", 3))
		perform_pwd(e);
	else if (!ft_strncmp(tab_cmd[0], "quit", 4))
		perform_quit(e);
	free_tab(tab_cmd);
}

void					set_pwd(t_env *e)
{
	char				*pwd;

	pwd = NULL;
	pwd = getwd(pwd);
	e->pwd = ft_strdup(pwd);
	e->curr_pwd = NULL;
	free(pwd);
}

char					*check_ls_cmd(char *cmd)
{
	int					i;
	int					j;
	char				*buff;

	if (!cmd)
		return (NULL);
	if (!(buff = malloc(sizeof(char) * (ft_strlen(cmd) + 1))))
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
		put_msg_on_fd(e->c_sock, to_send, 1);
		cmd = ft_strsplit(e->cmd, ' ');
		dup2(e->c_sock, 1);
		dup2(e->c_sock, 2);
		to_send = ft_strjoin("/bin/", cmd[0]);
		if (cmd[1] && cmd[1][0] != '-')
			cmd[1] = "";
		// cmd[1] = check_ls_cmd(cmd[1]);
		if (cmd[2])
			cmd[2] = "";
		execv(to_send, cmd);
		close(e->c_sock);
		free(to_send);
		free_tab(cmd);
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
		{
			perform_cmd(e);
			e->error = 0;
		}
		free(e->cmd);
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
