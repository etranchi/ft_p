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
	free(status);
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
	int i = -1;
	char **cmd;

	cmd = ft_strsplit(e->cmd, ' ');
	while (cmd[++i])
		printf("%s\n", cmd[i]);
	char *name_file = cmd[1];
	int size_file = ft_atoi(cmd[2]);
	int mode = ft_atoi(cmd[3]);
	printf("name_file %s\n", name_file);
	printf("size_file %d\n", size_file);

	if (access(name_file, F_OK) != -1)
		return error(e, "File already exist.\n");
	int fd;

	if ((fd = open(name_file, O_RDONLY | O_CREAT | O_WRONLY, mode)) < 0)
		return error(e, ft_strjoin("Can't create :", name_file));
	char buff[size_file];
	int len_read;
	if ((len_read = read(e->c_sock, buff, size_file)))
	{
		buff[len_read] = 0;
		write(fd, buff, len_read);
		printf("je finis d'écrire..\n");
	}


	send_es_to_client(e);
	// int file;
	// int all_put = 0;
	// file = fopen(name_file, "w");
	// while (all_put < size_file)
	// {
	// 	write(file, cmd[3], size_file);
	// 	all_put = size_file;
	// }
	// close(file);

}

void perform_pwd(t_env *e)
{
	send_es_to_client(e);
	put_on_client(e, e->curr_pwd);
}

void perform_quit(t_env *e)
{
	send_es_to_client(e);
	put_on_client(e, e->cmd);
}



void perform_cd(t_env *e, char **cmd)
{
	char *to_send;

	to_send = NULL;
	chdir(cmd[1] ? cmd[1] : ".");
	e->curr_pwd = getwd(e->curr_pwd);
	if (ft_strlen(e->curr_pwd) < ft_strlen(e->pwd))
	{
		chdir(e->pwd);
		e->curr_pwd = e->pwd;
		to_send = ft_strdup("ERROR : You don't have the rights to do this.");
		put_on_client(e, to_send);
	} else {
		to_send = ft_strjoin("cd to : ", e->curr_pwd);
		send_es_to_client(e);
		put_on_client(e, to_send);
	}
	
}

void perform_cmd(t_env *e)
{
	char **tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	printf("strstr %s\n", e->cmd);
	if (!ft_strncmp(e->cmd, "ls", 2)){
		printf("je go ls\n");
		perform_ls(e);
	}
	else if (!ft_strncmp(e->cmd, "cd", 2)){
		printf("je go cd\n");
		perform_cd(e, tab_cmd);
	}
	else if (!ft_strncmp(e->cmd, "get", 3)){
		printf("je go get\n");
		perform_get(e, tab_cmd);
	}
	else if (!ft_strncmp(e->cmd, "put", 3)){
		printf("je go put\n");
		perform_put(e);
	}
	else if (!ft_strncmp(e->cmd, "pwd", 3)){
		printf("je go pwd\n");
		perform_pwd(e);
	}
	else if (!ft_strncmp(e->cmd, "quit", 4)){
		printf("je go quit\n");
		perform_quit(e);
	}
	else {
		printf("je go else\n");
		e->cmd = ft_strjoin(e->cmd, " not found.");
		e->cmd = ft_strjoin("ERROR : ", e->cmd);
		put_on_client(e, e->cmd);
	}
}


void set_pwd(t_env *e)
{
	e->pwd = getwd(e->pwd);
	e->curr_pwd = ft_strdup(e->pwd);
}


// perform ls ok, need check leaks
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
    	close(e->c_sock);
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
	int size_line;

	if (ac < 2)
		error_exit("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
	e->port = ft_atoi(av[1]);
	create_server(e);
	while((size_line = get_next_line(e->c_sock, &e->cmd)) > 0)
	{
		perform_cmd(e);
		ft_bzero(e->cmd, ft_strlen(e->cmd));
		e->error = 0;
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
