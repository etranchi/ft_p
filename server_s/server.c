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



void put_on_client(t_env *e, char *msg);

void	error(char *reason)
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
		error("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error("Binding.\n");
	if (listen(e->sock, 42) == -1)
		error("Listen.\n");
	if ((e->c_sock = accept(e->sock, (struct sockaddr *)&csin, &cslen)) == -1)
		error("Accept.\n");
}


void	perform_ls(t_env *e, char **cmd);



void perform_get(t_env *e, char **cmd){(void)e; (void)cmd;}
void perform_put(t_env *e, char **cmd){(void)e; (void)cmd;}

void perform_pwd(t_env *e){
	char *to_send;
	to_send = ft_strjoin(e->curr_pwd, "\n");
	// write(e->c_sock, to_send, ft_strlen(to_send));
	put_on_client(e, to_send);
}

void perform_quit(t_env *e, char **cmd)
{
	// write(e->c_sock, cmd[0], ft_strlen(cmd[0]));
	put_on_client(e, cmd[0]);
	e->error = 1;
}

void put_on_client(t_env *e, char *msg)
{
	write(e->c_sock, msg, ft_strlen(msg));
}

void perform_cd(t_env *e, char **cmd)
{
	char *new_pwd;

	new_pwd = NULL;
	chdir(cmd[1] ? cmd[1] : ".");
	e->curr_pwd = getcwd(new_pwd, 0);
	if (ft_strlen(e->curr_pwd) < ft_strlen(e->pwd))
	{
		put_on_client(e, "You think i'm a fool ? You can't do this.\n");
		chdir(e->pwd);
		e->curr_pwd = e->pwd;
	} else {
		put_on_client(e, "ntd");	
	}
	
}

void perform_cmd(t_env *e, char *cmd)
{
	char **tab_cmd;

	tab_cmd = ft_strsplit(cmd, ' ');
	if (!ft_strcmp(tab_cmd[0], "ls"))
		perform_ls(e, tab_cmd);
	else if (!ft_strcmp(tab_cmd[0], "cd"))
		perform_cd(e, tab_cmd);
	else if (!ft_strcmp(tab_cmd[0], "get"))
		perform_get(e, tab_cmd);
	else if (!ft_strcmp(tab_cmd[0], "put"))
		perform_put(e, tab_cmd);
	else if (!ft_strcmp(tab_cmd[0], "pwd"))
		perform_pwd(e);
	else if (!ft_strcmp(tab_cmd[0], "quit"))
		perform_quit(e, tab_cmd);
	else 
		write(e->c_sock, "others\n", 7);
}


void set_pwd(t_env *e)
{
	char *buff;

	buff = NULL;

	buff = getcwd(buff, 0);
	e->pwd = buff;
	e->curr_pwd = ft_strdup(e->pwd);
}

void	perform_ls(t_env *e, char **cmd)
{
	pid_t pid;

 	pid = fork();

 	// needd to cmd[3] if there '..' can't ls parent folder..
	if (pid == -1)
    	error("Fork.\n");
    if (pid == 0)
    {
    	dup2(e->c_sock, 1);
    	close(e->c_sock);
    	cmd[0] = ft_strjoin("/bin/", cmd[0]);
    	if (cmd[2])
    		cmd[2] = e->pwd;
    	execv(cmd[0], cmd);
    	close(e->c_sock);
    }
	wait4(pid, 0, 0, NULL);
}

int		main(int ac, char **av)
{
	t_env *e;
	int size_line;
	char *line;

	if (ac < 2)
		error("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
	e->port = ft_atoi(av[1]);
	create_server(e);
	while(!e->error && (size_line = get_next_line(e->c_sock, &line)) > 0) 
		perform_cmd(e, line);
	printf("je vais close server\n");	
	close(e->c_sock);
	close(e->sock);
	return (0);
}
