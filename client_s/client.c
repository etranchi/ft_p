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

void						create_client(t_env *e, char *addr)
{
	struct protoent			*proto;
	struct sockaddr_in6		sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(AF_INET6, SOCK_STREAM, proto->p_proto);
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(e->port);
	inet_pton(AF_INET6, addr, &sin.sin6_addr);
	if (connect(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Connect.\n");
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

int							send_cmd_2(t_env *e, char *cmd)
{
	int ret;

	ret = 0;
	if (!ft_strcmp(cmd, "mkdir") && (ret = 1))
		send_mkdir(e);
	else if (!ft_strcmp(cmd, "rmdir") && (ret = 1))
		send_rmdir(e);
	else if (!ft_strcmp(cmd, "unlink") && (ret = 1))
		send_unlink(e);
	else if (!ft_strcmp(cmd, "get") && (ret = 1))
		get_file(e);
	else if (!ft_strcmp(cmd, "put") && (ret = 1))
		put_file(e);
	return (ret);
}

void						send_cmd(t_env *e)
{
	char					**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (send_cmd_2(e, tab_cmd[0]))
		return ;
	else if (!ft_strcmp(tab_cmd[0], "ls"))
		get_ls(e);
	else if (!ft_strcmp(tab_cmd[0], "cd"))
		get_cd(e);
	else if (!ft_strcmp(tab_cmd[0], "pwd"))
		get_pwd(e);
	else if (!ft_strcmp(tab_cmd[0], "lpwd"))
		perform_lpwd(e);
	else if (!ft_strcmp(tab_cmd[0], "lcd"))
		perform_lcd(e);
	else if (!ft_strcmp(tab_cmd[0], "lls"))
		perform_lls(e);
	else if (!ft_strcmp(tab_cmd[0], "quit"))
	{
		free_tab(tab_cmd);
		perform_quit();
	}
	free_tab(tab_cmd);
}

int							main(int ac, char **av)
{
	t_env					*e;

	if (ac < 3)
		error_exit("Usage ./client <addr> <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
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
