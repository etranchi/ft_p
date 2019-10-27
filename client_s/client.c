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

void						error(t_env *e, char *reason)
{
	ft_putstr("ERROR | ");
	ft_putstr(reason);
	ft_putstr("\n");
	e->error = 1;
}

void						create_client(t_env *e, char *addr)
{
	struct protoent			*proto;
	struct sockaddr_in		sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Connect.\n");
}

void						put_file(t_env *e)
{
	int						fd;
	char					**tab_cmd;
	struct stat				file_stat;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to put."));
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return (error(e, ft_strjoin("Can't open :", tab_cmd[1])));
	if (fd == -1)
		return (error(e, "File doesn't exist."));
	if (fstat(fd, &file_stat) < 0)
		return (error(e, "Fstat.\n"));
	if (!S_ISREG(file_stat.st_mode))
		return (error(e, "It's not a file."));
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, ft_itoa(file_stat.st_mode));
	put_msg_on_fd(e->sock, e->cmd);
	go_read_all_buff(e);
	if (e->error)
		return ;
	e->error = 1;
	read_fd_write_fd(e, fd, e->sock);
}

void						get_file(t_env *e)
{
	char					**tab_cmd;
	char					**file_info;
	int						fd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to put."));
	if (access(tab_cmd[1], F_OK) != -1)
		return (error(e, "File already exist."));
	put_msg_on_fd(e->sock, e->cmd);
	while ((e->len_read = read(e->sock, e->buff, 4095)))
	{
		e->buff[e->len_read] = '\0';
		if (e->len_read < 4095)
			break ;
	}
	file_info = ft_strsplit(e->buff, ' ');
	if (!ft_strncmp(file_info[0], "ERROR ", e->len_read > 6 ? 6 : e->len_read))
		return (error(e, file_info[0]));
	if ((fd = open(file_info[1], O_RDONLY | O_CREAT | O_WRONLY,
		ft_atoi(file_info[2]))) < 0)
		return (error(e, "Can't create file."));
	read_fd_write_fd(e, e->sock, fd);
	printf("SUCCESS | Created %s\n", file_info[1]);
	e->error = 1;
}

void						send_cmd(t_env *e)
{
	char					**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strcmp(tab_cmd[0], "put"))
		put_file(e);
	else if (!ft_strcmp(tab_cmd[0], "get"))
		get_file(e);
	else
		put_msg_on_fd(e->sock, e->cmd);
}

void						prompt(void)
{
	ft_putstr("<$(.)(.)$>");
}

void						go_read_all_buff(t_env *e)
{
	while ((e->len_read = read(e->sock, e->buff, 4095)) > 0)
	{
		e->buff[e->len_read] = '\0';
		write(1, e->buff, e->len_read);
		if (!ft_strncmp(e->buff, "ERROR ", 6) && (e->error = 1))
			return ;
		if (!ft_strncmp(e->buff, "SUCCESS | ls", 12) ||
			!ft_strncmp(e->buff, "/bin/ls: illegal", 16))
			return (go_read_all_buff(e));
		ft_bzero(e->buff, e->len_read);
		if (e->len_read < 4095)
			break ;
	}
	e->len_read = 0;
}

int							main(int ac, char **av)
{
	t_env					*e;

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
		if (ft_strlen(e->cmd) > 0)
		{
			send_cmd(e);
			if (!e->error)
				go_read_all_buff(e);
		}
		prompt();
		e->error = 0;
		e->len_read = 0;
	}
	close(e->sock);
	return (0);
}
