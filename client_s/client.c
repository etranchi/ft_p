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

void prompt(void)
{
	ft_putstr("(.)(.)$>");
}

void	*ft_memncpy(void *dst, const void *src, int start,size_t n)
{
	char	*d;
	char	*s;

	d = (char *)dst;
	s = (char *)src;
	d = d + start;
	while (n--)
	{
		*d = *s;
		d++;
		s++;
	}
	return (dst);
}

void my_join(char *s1, char *s2, int size_read)
{
	char *tmp;

	if (!s1 && s2){
		s1 = ft_memalloc(size_read);
		ft_memcpy(s1, s2, size_read);
		return ;
	}
	if (s1 && s2) 
	{
		tmp = s1;
		s1 = ft_memalloc(size_read + ft_strlen(tmp));
		s1 = ft_memcpy(s1,tmp, ft_strlen(tmp));
		s1 = ft_memncpy(s1,s2, ft_strlen(s1), ft_strlen(tmp));
		s1[ft_strlen(s1) + ft_strlen(tmp)] = 0;
		return ;
	}
	printf("je dois pas passer ici normalement\n");

}

void go_read_all_buff(t_env *e)
{
	char buff[4096];
	int size_read;

	e->res = NULL;
	while((size_read = read(e->sock, buff, 4095)) > 0)
	{
		buff[size_read] = '\0';
		if (size_read >= 3 && !ft_strncmp(buff, "ntd", 3))
		{ // ntd = nothing to do
			printf("je passe la\n");
			break ;
		}
			
		if (size_read >= 4 && !ft_strncmp(buff, "quit", 4))
		{
			printf("je passe ici\n");
			close(e->sock);
			exit(1);
		}
		printf("%s",buff);
		if (size_read < 4095)
			break ;
	}
}

int		main(int ac, char **av)
{
	t_env	*e;
	
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
		check_cmd(e);
		if (!e->error)
			go_read_all_buff(e);
		prompt();
	}
		
	close(e->sock);
	return (0);
}