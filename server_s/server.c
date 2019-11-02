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
	struct sockaddr_in6	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(AF_INET6, SOCK_STREAM, proto->p_proto);
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(e->port);
	sin.sin6_addr = in6addr_any;
	if (bind(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Binding.\n");
	if (listen(e->sock, 42) == -1)
		error_exit("Listen.\n");
}

int						perform_cmd_2(t_env *e, char *cmd)
{
	if (!ft_strcmp(cmd, "mkdir"))
		perform_mkdir(e);
	else if (!ft_strcmp(cmd, "rmdir"))
		perform_rmdir(e);
	else if (!ft_strcmp(cmd, "unlink"))
		perform_unlink(e);
	return (0);
}

void					perform_cmd(t_env *e)
{
	char				**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strcmp(tab_cmd[0], "ls"))
		perform_ls(e);
	else if (!ft_strcmp(tab_cmd[0], "cd"))
		perform_cd(e);
	else if (!ft_strcmp(tab_cmd[0], "get"))
		perform_get(e);
	else if (!ft_strcmp(tab_cmd[0], "put"))
		perform_put(e);
	else if (!ft_strcmp(tab_cmd[0], "pwd"))
		perform_pwd(e);
	else if (perform_cmd_2(e, tab_cmd[0]))
		return ;
	free_tab(tab_cmd);
}

void					loop(t_env *e)
{
	unsigned int		cslen;
	struct sockaddr_in6	csin;
	pid_t				pid;

	while (42)
	{
		if ((e->c_sock = accept(e->sock, (struct sockaddr *)&csin, &cslen)) < 0)
			error_exit("Accept.\n");
		if((pid = fork()) == 0)
		{
			close(e->sock);	
			while ((get_next_line(e->c_sock, &e->cmd)) > 0)
				if (e->cmd && ft_strlen(e->cmd) > 0)
				{
					perform_cmd(e);
					e->error = 0;
					free(e->cmd);
				}
			break ;
		}
	}
}

int						main(int ac, char **av)
{
	t_env				*e;
	
	if (ac < 2)
		error_exit("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	init_env(e, av[1]);
	loop(e);
	close(e->c_sock);
	close(e->sock);
	return (0);
}
