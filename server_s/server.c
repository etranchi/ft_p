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

int		error(char *err)
{
	ft_putstr(err);
	return (ERROR);
}


int		create_server(int port)
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
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error("Error binding\n");
	listen(sock, 42);
	return (sock);
}

int		main(int ac, char **av)
{
	int port;
	int sock;
	unsigned int cslen;
	struct sockaddr_in csin;
	int size_line;
	int cs;
	char line[100];
	if (ac < 2)
		return error("Usage ./server <port>\n");
	port = ft_atoi(av[1]);
	sock = create_server(port);
	cs = accept(sock, (struct sockaddr *)&csin, &cslen);
	while((size_line = read(cs, &line, 99)) > 0)
	{
		line[size_line] = 0;
		ft_putstr(line);
		write(1, "\n", 1);
	}
	close(cs);
	close(sock);
	return (0);
}
