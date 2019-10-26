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


void put_on_client(t_env *e, char *msg)
{
	char *to_send;

	to_send = ft_strjoin(msg, "\n");
	write(e->c_sock, to_send, ft_strlen(to_send));
// 	free(msg);
//	free(to_send);
}

void send_es_to_client(t_env *e)
{
	char *to_send;
	char *status;

	status = e->error ? ft_strdup("ERROR | ") : ft_strdup("SUCCESS | ");
	to_send = ft_strjoin(status, e->cmd);
	put_on_client(e, to_send);
	// free(status);
}	

void	error(t_env *e, char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	e->error = 1;
}

void	error_exit(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}

void	create_server(t_env *e)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;
	unsigned int 		cslen;
	struct sockaddr_in csin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
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



void perform_get(t_env *e, char **cmd){(void)e; (void)cmd;}

void perform_put(t_env *e){
	char **cmd;
	int i;
	int fd;

	i = -1;
	cmd = ft_strsplit(e->cmd, ' ');
	if (access(cmd[1], F_OK) != -1)
		return put_on_client(e, "ERROR | File already exist.");
	if ((fd = open(cmd[1], O_RDONLY | O_CREAT | O_WRONLY, ft_atoi(cmd[2]))) < 0)
		return put_on_client(e, "ERROR | Can't create file.");
	char *to_send = ft_strjoin("SUCCESS | Created ", cmd[1]);
	put_on_client(e, to_send);
	while ((e->len_read = read(e->c_sock, e->buff, 4095)) > 0)
	{
		e->buff[e->len_read] = '\0';
		printf("%s\n", e->buff);
		write(fd, e->buff, e->len_read);
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < 4095)
			break ;
	}
	e->len_read = 0;
}

void perform_pwd(t_env *e)
{
	char *to_send = ft_strjoin("SUCCESS | pwd | ", e->curr_pwd);
	put_on_client(e, to_send);
}

void perform_quit(t_env *e)
{
	char *to_send = ft_strdup("SUCCESS | quit");
	put_on_client(e, to_send);
}



void perform_cd(t_env *e, char **cmd)
{
	char *to_send;
	char *save_pwd;

	chdir(cmd[1] ? cmd[1] : ".");
	save_pwd = e->curr_pwd;
	e->curr_pwd = getcwd(NULL, 0);
	if (ft_strlen(e->curr_pwd) < ft_strlen(e->pwd))
	{
		to_send = ft_strdup("ERROR : You don't have the rights to do this.");
		put_on_client(e, to_send);
		chdir(save_pwd);
		e->curr_pwd = save_pwd;
	} else {
		if (!cmd[1])
			e->curr_pwd = e->pwd;
		char *to_send = ft_strjoin("SUCCESS | cd | ", e->curr_pwd);
		put_on_client(e, to_send);
		chdir(e->curr_pwd);
	}
}

void perform_cmd(t_env *e)
{
	char **tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strncmp(tab_cmd[0], "ls", 2))
		perform_ls(e);
	else if (!ft_strncmp(tab_cmd[0], "cd", 2))
		perform_cd(e, tab_cmd);
	else if (!ft_strncmp(tab_cmd[0], "get", 3))
		perform_get(e, tab_cmd);
	else if (!ft_strncmp(tab_cmd[0], "put", 3))
		perform_put(e);
	else if (!ft_strncmp(tab_cmd[0], "pwd", 3))
		perform_pwd(e);
	else if (!ft_strncmp(tab_cmd[0], "quit", 4))
		perform_quit(e);
	else {
		e->cmd = ft_strjoin(e->cmd, " not found.");
		e->cmd = ft_strjoin("ERROR : ", e->cmd);
		put_on_client(e, e->cmd);
	}
}


void set_pwd(t_env *e)
{
	char *pwd;

	pwd = NULL;
	pwd = getwd(pwd);
	e->pwd = ft_strdup(pwd);
	e->curr_pwd = ft_strdup(e->pwd);
}

void	perform_ls(t_env *e)
{
	pid_t pid;
	char **cmd;

 	pid = fork();
	if (pid == -1)
    	error_exit("Fork.\n");
    if (pid == 0)
    {
    	send_es_to_client(e);
    	cmd = ft_strsplit(e->cmd, ' ');
    	dup2(e->c_sock, 1);
    	dup2(e->c_sock, 2);
    	cmd[0] = ft_strjoin("/bin/", cmd[0]);
    	if (cmd[1] && ft_strstr(cmd[1], "..")) {
    		cmd[1] = NULL;
    	}
    	if (cmd[2])
    		cmd[2] = e->curr_pwd;
    	execv(cmd[0], cmd);
    	close(e->c_sock);
    }
	wait4(pid, 0, 0, NULL);
}


int		main(int ac, char **av)
{
	t_env *e;

	if (ac < 2)
		error_exit("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
	e->port = ft_atoi(av[1]);
	create_server(e);
	while((get_next_line(e->c_sock, &e->cmd)) > 0)
	{
		perform_cmd(e);
		ft_bzero(e->cmd, ft_strlen(e->cmd));
		e->error = 0;
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
