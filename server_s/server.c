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

void	error(char *reason)
{
	ft_putstr("ERROR : ");
	ft_putstr(reason);
	exit(1);
}


void	create_server(t_env *e)
{
	struct protoent		*proto;
	struct sockaddr_in	sin;
	unsigned int 		cslen;
	struct sockaddr_in csin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		error("Proto.\n");
	e->sock = socket(PF_INET, SOCK_STREAM,	proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(e->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(e->sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		error("Binding.\n");
	if (listen(e->sock, 42) == -1)
		error("Listen.\n");
	if ((e->c_sock = accept(e->sock, (struct sockaddr *)&csin, &cslen)) == -1)
		error("Accept.\n");
}


void send_res(t_env *e, int size) 
{
	if (e->res && ft_strlen(e->res))
	{	
		//int i = -1;
		// char buf[4096];
		//int j = -1;
		// while (e->res[++i] || i < size)
		// {
		// 	printf("%i %c \n", e->res[i], e->res[i]);
		// }
		
		printf("%s", e->res);
		// e->res = ft_strjoin(e->res, "\0");
		e->res[size - 1] = 0;
		write(e->c_sock, e->res, size);
		printf("Sending to client...\n");
	}
}

void	perform_cmd(t_env *e, char *cmd)
{
	int link[2];
	pid_t pid;
 	char res[4096];

 	(void)e;
 	if (pipe(link) == -1)
 		error("Pipe.\n");
	if ((pid = fork()) == -1)
    	error("Fork.\n");
    if (pid == 0)
    {
    	dup2 (link[1], STDOUT_FILENO);
    	close(link[0]);
    	close(link[1]);
    	/// getpwd()
    	execl("/bin/ls", cmd, "-1", (char *)0);
    	exit(1);
    }
	int size_read;
	while (1) 
	{
		size_read = read(link[0], res, sizeof(res));
		if (size_read == -1)
			exit(1);
		else if (size_read == 0)
			break ;
		else
		{
			res[size_read] = 0;
			e->res = res;
			send_res(e, size_read);
			break ;
		}
	}
	close(link[0]);
	wait(0);
    
    /*else
    {
		close(link[1]);
		int nbytes = 0;
		if (pid) {
			int i = 0;
			while ((nbytes = read(link[0], res, sizeof(res))) > 0){
				printf("(%d) %d\n", i++, pid);
	    		printf("Output: (%.*s)\n", nbytes, res);
	    		e->res = ft_strjoin(e->res, res);
	    		send_res(e);
	    	}
	    	
	    }
    }*/
    // close(link[0]);
    // wait(0);
}

int		main(int ac, char **av)
{
	t_env *e;
	int size_line;
	char *line;

	// char bjr[100] = "Bonjour, je suis le serveur\n";

	if (ac < 2)
		error("Usage ./server <port>\n");
	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error("Malloc.\n");
	ft_memset(e, 0, sizeof(t_env));
	e->port = ft_atoi(av[1]);
	create_server(e);
	while((size_line = get_next_line(e->c_sock, &line)) > 0) 
	{
		// ft_putstr(line);
		// write(1, "\n", 1);
		perform_cmd(e, line);		
	}
	close(e->c_sock);
	close(e->sock);
	return (0);
}
