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
	bind(sock, (const struct sockaddr *)&sin, sizeof(sin));
	listen(sock, 42);

	return (sock);
}

int		main(int ac, char **av) {
	int port;
	int sock;
	unsigned int cslen;
	struct sockaddr_in csin;
	int r;
	int cs;
	char buf[100];
	if (ac < 2)
		return error("No args");
	port = ft_atoi(av[1]);
	sock = create_server(port);
	cs = accept(sock, (struct sockaddr *)&csin, &cslen);
	r = read(cs, &buf, 99);
	if (r > 0)
	{
		ft_putstr(buf);
	}
	close(cs);
	close(sock);
	return (0);
}
