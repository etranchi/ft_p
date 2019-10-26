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

void	error_exit(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}

void put_on_server(t_env *e, char *msg)
{
	char *to_send = ft_strjoin(msg, "\n");
	write(e->sock, to_send, ft_strlen(to_send));
}

void	error(t_env *e, char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	e->error = 1;
}

void go_read_all_buff(t_env *e);

void	create_client(t_env *e, char *addr)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error_exit("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = inet_addr(addr);
	if (connect(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error_exit("Connect.\n");
}


void send_cmd(t_env *e);

void put_file(t_env *e)
{
	int fd;
	char **tab_cmd;
	struct stat file_stat;
	char *mode;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if ((fd = open(tab_cmd[1], O_RDONLY)) < 0)
		return error(e, ft_strjoin("Can't open :", tab_cmd[1]));
	if (fd == -1)
		return error(e,"ERROR : file doesn't exist.\n");
	if (fstat(fd, &file_stat) < 0)
		return error(e, "Fstat.\n");
	mode = ft_itoa(file_stat.st_mode);
	if (!S_ISREG(file_stat.st_mode))
		return error(e, "ERROR : it's not a file.\n");
	printf("File size %lld\n", file_stat.st_size);

	// int len = send(e->sock, file_stat.st_size, sizeof(file_stat.st_size));
	// printf("len = %d\n", len);
	// if (len < 0)
	// 	error("Failed to send file.\n");

	char *size_file = ft_itoa(file_stat.st_size);
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, size_file);
	e->cmd = ft_strjoin(e->cmd, " ");
	e->cmd = ft_strjoin(e->cmd, mode);
	send_cmd(e);

	char buff[file_stat.st_size];
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


	 // e->error = 1;// debug
}

void send_cmd(t_env *e) 
{
	put_on_server(e, e->cmd);
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

}

void go_read_all_buff(t_env *e)
{
	char buff[4096];
	int size_read;
	char *es_msg;

	if (get_next_line(e->sock, &es_msg) > 0)
	{
		printf("%s\n", es_msg);
		if (ft_strstr(es_msg, "SUCCESS | ") || ft_strstr(es_msg, "ERROR | "))
		{
			printf("je passe les strstr\n");
		 	if (printf("c\n") && (size_read = read(e->sock, buff, 4095)) > 0 && printf("d\n"))
			{
				printf("je read\n");
				buff[size_read] = '\0';
				printf("%s", buff);
				ft_bzero(buff, size_read);
				if (size_read < 4095)
					return ;
			}
		}
	}

	
}

int		main(int ac, char **av)
{
	t_env	*e;
	
	if (ac < 3)
		error_exit("Usage ./client <addr> <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_exit("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	e->port = ft_atoi(av[2]);
	create_client(e, av[1]);
	prompt();
	while (get_next_line(STDOUT, &e->cmd) > 0)
	{
		send_cmd(e);
		go_read_all_buff(e);
		prompt();

		// e->error = 0;
	}
		
	close(e->sock);
	return (0);
}