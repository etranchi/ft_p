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

void go_read_all_buff(t_env *e);

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


void send_cmd(t_env *e);

void put_file(t_env *e)
{
	// char *line;
	int fd;
	char **tab_cmd;
	struct stat file_stat;

	printf("je suis dans put cmd : %s\n", e->cmd);
	tab_cmd = ft_strsplit(e->cmd, ' ');
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		error(ft_strjoin("Can't open :", tab_cmd[1]));
	if (fd == -1)
	{
		e->error = 1;
		return ft_putstr("ERROR : file doesn't exist.\n");
	}
	printf("je vais put :%s\n", tab_cmd[1]);
	if (fstat(fd, &file_stat) < 0)
		error("Fstat.\n");
	if (!S_ISREG(file_stat.st_mode)) {
		e->error = 1;
		return ft_putstr("ERROR : it's not a file.\n");
	}
	printf("File size %lld\n", file_stat.st_size);

	// int len = send(e->sock, file_stat.st_size, sizeof(file_stat.st_size));
	// printf("len = %d\n", len);
	// if (len < 0)
	// 	error("Failed to send file.\n");

	char *tmp_int = ft_itoa(file_stat.st_size);
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, tmp_int);
	send_cmd(e);
	char buff[file_stat.st_size];
	go_read_all_buff(e);
	int len_read;
	if ((len_read = read(fd, buff, file_stat.st_size))) 
	{
		buff[len_read] = 0;
		printf("file : %s\n", buff);
		write(e->sock, buff, file_stat.st_size);

	}
	// while (size_send <= file_stat.st_size)
	// {	

	// 	write(e->sock, )
	// }


	// write(e->sock, buff, len);


	e->error = 1; // debug
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
				if (!ft_strncmp(CMDS[i], "put", CMDS_LEN)) {
					put_file(e);
				}
				else {
					send_cmd(e);
				}
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
			break ;
		}
			
		if (size_read >= 4 && !ft_strncmp(buff, "quit", 4))
		{
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
		e->error = 0;
	}
		
	close(e->sock);
	return (0);
}