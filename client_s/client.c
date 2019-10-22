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

void	error(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}


void	create_client(t_env *e, char *addr)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error("Connect.\n");
}

void send_cmd(t_env *e) 
{
	e->cmd = ft_strjoin(e->cmd, "\n");	
	write(e->sock, e->cmd, ft_strlen(e->cmd));

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
			printf("cmd_len ; %zu, CMDS_len : %zu \n", cmd_len, CMDS_LEN);
			if ((cmd_len == CMDS_LEN) | (cmd_len > CMDS_LEN && e->cmd[CMDS_LEN] == ' '))
			{
				send_cmd(e);
				return ;
			}
		}
	}
	ft_putstr("ERROR : wrong command.\n");
	e->error = 1;
	return ;
}

void prompt(void) {
	ft_putstr("(.)(.)$>");
}

int		main(int ac, char **av)
{
	t_env	*e;
	char line[4096];
	int size_line;

	
	if (ac < 3)
		error("Usage ./client <addr> <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	e->port = ft_atoi(av[2]);
	create_client(e, av[1]);
	prompt();
	while (get_next_line(STDOUT, &e->cmd) > 0)
	{
		printf("la ?\n");
		check_cmd(e);
		while (!e->error && (size_line = read(e->sock, line, 4095)) > 0)
		{
			printf("(%d)\n", size_line);
			ft_putstr("Response : ");
			printf("%s\n", line);
			printf("je me perds");
			break ;
		}
		printf("ici ?\n");
		prompt();
	}
	close(e->sock);
	return (0);
}