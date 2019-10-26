/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 15:41:47 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/21 15:41:49 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void	error_exit(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}

void put_on_server(t_env *e, char *msg)
{
	char *to_send = ft_strjoin(msg, "\n");
	write(e->sock, to_send, ft_strlen(to_send));
}

void	error(t_env *e, char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	(void)e;
}



void	create_client(t_env *e, char *addr)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Connect.\n");
}


void send_cmd(t_env *e);

void put_file(t_env *e)
{
	int fd;
	char **tab_cmd;
	struct stat file_stat;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return error(e, ft_strjoin("Can't open :", tab_cmd[1]));
	if (fd == -1)
		return error(e,"ERROR : file doesn't exist.\n");
	if (fstat(fd, &file_stat) < 0)
		return error(e, "Fstat.\n");
	if (!S_ISREG(file_stat.st_mode))
		return error(e, "ERROR : it's not a file.\n");
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, ft_itoa(file_stat.st_mode));
	put_on_server(e, e->cmd);
	while ((e->len_read = read(fd, e->buff, 4095)) > 0) 
	{
		e->buff[e->len_read] = 0;
		write(e->sock, e->buff, e->len_read);
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < 4095)
			break ;
	}
	printf("je finis put\n");
}

void send_cmd(t_env *e) 
{
	if (!ft_strncmp(e->cmd, "put ", 4))
	{
		put_file(e);
	}
	else {
		put_on_server(e, e->cmd);	
	}
	

}


void prompt(void)
{
	ft_putstr("(.)(.)$>");
}

void go_read_all_buff(t_env *e)
{
	while((e->len_read = read(e->sock, e->buff, 4095)) > 0)
	{
		e->buff[e->len_read] = '\0';
		write(1, e->buff, e->len_read);
		if (!ft_strncmp(e->buff, "SUCCESS | ls", 12))
			return go_read_all_buff(e);
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < 4095)
			break ;
	}
	e->len_read = 0;
}

int		main(int ac, char **av)
{
	t_env	*e;
	
	if (ac < 3)
		error_exit("Usage ./client <addr> <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	e->port = ft_atoi(av[2]);
	create_client(e, av[1]);
	prompt();
	while (get_next_line(STDOUT, &e->cmd) > 0)
	{
		send_cmd(e);
		go_read_all_buff(e);
		prompt();
	}
	close(e->sock);
	return (0);
}