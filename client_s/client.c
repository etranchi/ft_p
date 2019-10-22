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

int		error(char *err)
{
	ft_putstr(err);
	return (ERROR);
}


int		create_client(char *addr, int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (ERROR);
	sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error("Error Connection\n");
	return (sock);
}

void send_cmd(char *cmd) 
{
	ft_putstr("3");
	ft_putstr(cmd);
	ft_putstr("4");
}

int check_cmd(char *cmd)
{
	int i;

	i = -1;
	while (CMDS[++i])
	{
		ft_putstr(CMDS[i]);

		printf("%s %zu\n", cmd, ft_strlen(cmd));
		if (cmd && ft_strlen(cmd) && !ft_strcmp(CMDS[i], cmd)) {
			ft_putstr("1");
			send_cmd(cmd);
			ft_putstr("2");		
		}
	}
	return (0);
}

int		main(int ac, char **av)
{
	int port;
	int sock;
	char *line;

	if (ac < 3)
		return error("Usage ./client <addr> <port>\n");
	port = ft_atoi(av[2]);
	sock = create_client(av[1],port);
	while (get_next_line(1, &line) > 0)
	{
		write(1, line, ft_strlen(line));
		check_cmd(line);
		ft_putstr("5");
		write(sock, line, ft_strlen(line));
		ft_putstr("6");
	}
	close(sock);
	return (0);
}