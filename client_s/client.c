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
	e->error = 1;
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
	char *mode;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return error(e, ft_strjoin("Can't open :", tab_cmd[1]));
	if (fd == -1)
		return error(e,"ERROR : file doesn't exist.\n");
	if (fstat(fd, &file_stat) < 0)
		return error(e, "Fstat.\n");
	mode = ft_itoa(file_stat.st_mode);
	if (!S_ISREG(file_stat.st_mode))
		return error(e, "ERROR : it's not a file.\n");
	printf("File size %lld\n", file_stat.st_size);

	// int len = send(e->sock, file_stat.st_size, sizeof(file_stat.st_size));
	// printf("len = %d\n", len);
	// if (len < 0)
	// 	error("Failed to send file.\n");

	char *size_file = ft_itoa(file_stat.st_size);
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, size_file);
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, mode);
	printf("je send %s\n", e->cmd);
	put_on_server(e, e->cmd);

	char buff[4096];
	int len_read;
	while ((len_read = read(fd, buff, 4095)) > 0) 
	{
		buff[len_read] = 0;
		printf("file : %s\n", buff);
		write(e->sock, buff, len_read);
		ft_bzero(buff, 4095);
		if (len_read < 4095)
			break ;
	}
	// while (size_send <= file_stat.st_size)
	// {	

	// 	write(e->sock, )
	// }


	// write(e->sock, buff, len);


	 // e->error = 1;// debug
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
	char buff[4096];
	int size_read;

	while((size_read = read(e->sock, buff, 4095)) > 0)
	{
		buff[size_read] = '\0';
		write(1, buff, size_read);
		if (!ft_strncmp(buff, "SUCCESS | ls", 12)){
			go_read_all_buff(e);
			break ;
		}
		if (size_read < 4095)
			break ;
	}
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