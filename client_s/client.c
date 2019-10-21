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
		error("ERROR Connection\n");
	return (sock);
}

int		main(int ac, char **av)
{
	int port;
	int sock;

	if (ac < 3)
		return error("Usage ./client <addr> <port>\n");
	port = ft_atoi(av[2]);
	sock = create_client(av[1],port);
	write(sock, "coucou\n", 7);
	close(sock);
	return (0);
}