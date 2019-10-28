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

void						error(t_env *e, char *reason, char **tab)
{
	ft_putstr("ERROR | ");
	ft_putstr(reason);
	ft_putstr("\n");
	e->error = 1;
	free_tab(tab);
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
	char					*to_send;
	t_data					*data;


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
	to_send = ft_strjoin(to_send, ft_itoa(file_stat.st_mode));
	put_msg_on_fd(e->sock, to_send, 1);
	read_fd_write_fd(e, fd, e->sock);
	data = read_fd(e, e->sock);
	check_data(e, data, STDOUT);
	if (!e->error)
		free_data(data);

}

void						get_file(t_env *e)
{
	char					**tab_cmd;
	char					**file_info;
	int						fd;
	t_data					*data;
	char					*to_send;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to put.", tab_cmd));
	if (access(tab_cmd[1], F_OK) != -1)
		return (error(e, "File already exist.",tab_cmd));
	to_send = ft_strdup(e->cmd);
	put_msg_on_fd(e->sock, to_send, 1);
	data = read_fd(e, e->sock);
	check_data(e, data, STDOUT);
	free_tab(tab_cmd);
	if (e->error)
		return ;
	file_info = ft_strsplit(data->data, ' ');
	if ((fd = open(file_info[1], O_RDONLY | O_CREAT | O_WRONLY, ft_atoi(file_info[2]))) < 0)
		return (error(e, "Can't create file.", file_info));
	read_fd_write_fd(e, e->sock, fd);
	printf("SUCCESS | Created %s\n", file_info[1]);
	free_tab(file_info);
	free_data(data);
	e->error = 1;
}



void						get_ls(t_env *e)
{
	char					*to_send;

	to_send = ft_strdup(e->cmd);
	put_msg_on_fd(e->sock, to_send, 1);
	go_read_all_buff(e);
}

void check_cmd(t_env *e)
{
	int i;
	size_t cmd_len;
	size_t CMDS_LEN;

	i = -1;
	e->error = 0;
	while (CMDS[++i] && e->cmd)
	{
		cmd_len = ft_strlen(e->cmd);
		CMDS_LEN = ft_strlen(CMDS[i]);
		if (!ft_strncmp(CMDS[i], e->cmd, CMDS_LEN))
		{
			if ((cmd_len == CMDS_LEN) | (cmd_len > CMDS_LEN && e->cmd[CMDS_LEN] == ' '))
				return ;
		}
	}
	ft_putstr("ERROR | wrong command.\n");
	e->error = 1;
	return ;
}


void						send_cmd(t_env *e)
{
	char					**tab_cmd;
	char					*to_send;
	t_data					*data;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strcmp(tab_cmd[0], "put"))
		put_file(e);
	else if (!ft_strcmp(tab_cmd[0], "get"))
		get_file(e);
	else if (!ft_strcmp(tab_cmd[0], "ls"))
		get_ls(e);
	else
	{
		check_cmd(e);
		if (e->error)
			return ;
		to_send = ft_strdup(e->cmd);
		put_msg_on_fd(e->sock, to_send, 1);
		data = read_fd(e, e->sock);
		check_data(e, data, STDOUT);
		if (!e->error)
			free_data(data);
	}
	free_tab(tab_cmd);
}

void						prompt(void)
{
	ft_putstr("<$(.)(.)$>");
}

void						go_read_all_buff(t_env *e)
{
	t_data *data;

	data = read_fd(e, e->sock);
	write(1, data->data, data->size);
	check_data(e, data, -1);
	if (e->error)
	{
		free_data(data);
		return ;
	}
	if (!ft_strncmp(data->data, "SUCCESS | ls", MIN(12, data->size)) || !ft_strncmp(data->data, "/bin/ls: illegal", MIN(16, data->size)))
	{
		free_data(data);
		return go_read_all_buff(e);
	}
	free_data(data);
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
	while ((get_next_line(STDOUT, &e->cmd)) > 0)
	{
		if (ft_strlen(e->cmd) > 0)
			send_cmd(e);
		free(e->cmd);
		prompt();
		e->error = 0;
	}
	close(e->sock);
	return (0);
}
