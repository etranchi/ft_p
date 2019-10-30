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
	struct sockaddr_in	sin;
	unsigned int		cslen;
	struct sockaddr_in	csin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
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

void					perform_quit(t_env *e)
{
	put_msg_on_fd(e->c_sock, "SUCCESS | quit", 0);
}

void					perform_cmd(t_env *e)
{
	char				**tab_cmd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (!ft_strncmp(tab_cmd[0], "ls", 2))
		perform_ls(e);
	else if (!ft_strncmp(tab_cmd[0], "cd", 2))
		perform_cd(e);
	else if (!ft_strncmp(tab_cmd[0], "get", 3))
		perform_get(e);
	else if (!ft_strncmp(tab_cmd[0], "put", 3))
		perform_put(e);
	else if (!ft_strncmp(tab_cmd[0], "pwd", 3))
		perform_pwd(e);
	else if (!ft_strncmp(tab_cmd[0], "quit", 4))
		perform_quit(e);
	free_tab(tab_cmd);
}

char					*check_ls_cmd(char *cmd)
{
	int					i;
	int					j;
	char				*buff;

	if (!cmd)
		return (NULL);
	if (!(buff = malloc(sizeof(char) * (ft_strlen(cmd) + 1))))
		error_exit("Malloc.\n");
	i = 0;
	j = 0;
	buff[0] = cmd[0];
	while (cmd[++i])
		if (ft_isdigit(cmd[i]))
			continue ;
		else
			buff[++j] = cmd[i];
	buff[++j] = '\0';
	return (buff);
}

int						main(int ac, char **av)
{
	t_env				*e;

	if (ac < 2)
		error_exit("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	set_pwd(e);
	e->port = ft_atoi(av[1]);
	create_server(e);
	while ((get_next_line(e->c_sock, &e->cmd)) > 0)
	{
		if (ft_strlen(e->cmd) > 0)
		{
			perform_cmd(e);
			e->error = 0;
		}
		free(e->cmd);
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
