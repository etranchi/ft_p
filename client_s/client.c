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
	(void)e;
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
	to_send = ft_strjoin_free(to_send, ft_itoa(file_stat.st_mode), 1, 1);
	put_msg_on_fd(e->sock, to_send, 1);
	if (!get_status_fd(e->sock, STDOUT))
		return ;
	read_fd_write_fd(e, fd, e->sock);
}

void						get_file(t_env *e)
{
	char					**tab_cmd;
	char					**file_info;
	int						fd;
	t_data					*data;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!tab_cmd[1])
		return (error(e, "No file to get.", tab_cmd));
	if (access(tab_cmd[1], F_OK) != -1)
		return (error(e, "File already exist.", tab_cmd));
	put_msg_on_fd(e->sock, e->cmd, 0);
	free_tab(tab_cmd);
	if (!get_status_fd(e->sock, STDOUT))
		return ;
	if ((data = read_fd(e, e->sock)) == NULL)
		return ;
	file_info = ft_strsplit(data->data, ' ');
	if ((fd = open(file_info[1], O_RDONLY | O_CREAT | O_WRONLY,
		ft_atoi(file_info[2]))) < 0)
		return (error(e, "Can't create file.", file_info));
	put_msg_on_fd(e->sock, "SUCCESS", 0);
	read_fd_write_fd(e, e->sock, fd);
	printf("SUCCESS | Created %s\n", file_info[1]);
	free_tab(file_info);
	free_data(data);
}

void						get_ls(t_env *e)
{
	t_data					*data;

	put_msg_on_fd(e->sock, e->cmd, 0);
	data = read_fd(e, e->sock);
	write_data_on_fd(e, data, STDOUT);
	read_fd_write_fd(e, e->sock, STDOUT);
}

int							check_cmd(t_env *e)
{
	int						i;
	size_t					cmd_len;
	size_t					cmds_len;

	i = -1;
	while (CMDS[++i] && e->cmd)
	{
		cmd_len = ft_strlen(e->cmd);
		cmds_len = ft_strlen(CMDS[i]);
		if (!ft_strncmp(CMDS[i], e->cmd, cmds_len))
		{
			if ((cmd_len == cmds_len) ||
				(cmd_len > cmds_len && e->cmd[cmds_len] == ' '))
				return (1);
		}
	}
	ft_putstr("ERROR | wrong command.\n");
	return (0);
}

void						get_cd(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
}

void						get_pwd(t_env *e)
{
	put_msg_on_fd(e->sock, e->cmd, 0);
	read_fd_write_fd(e, e->sock, STDOUT);
	read_fd_write_fd(e, e->sock, STDOUT);
}

void						send_cmd(t_env *e)
{
	char					**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strcmp(tab_cmd[0], "put"))
		put_file(e);
	if (!ft_strcmp(tab_cmd[0], "get"))
		get_file(e);
	else if (!ft_strcmp(tab_cmd[0], "ls"))
		get_ls(e);
	else if (!ft_strcmp(tab_cmd[0], "cd"))
		get_cd(e);
	else if (!ft_strcmp(tab_cmd[0], "pwd"))
		get_pwd(e);
	free_tab(tab_cmd);
}

void						prompt(void)
{
	ft_putstr("<$(.)(.)$>");
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
		if (ft_strlen(e->cmd) > 0 && check_cmd(e))
		{
			send_cmd(e);
			e->error = 0;
		}
		free(e->cmd);
		prompt();
	}
	close(e->sock);
	return (0);
}
