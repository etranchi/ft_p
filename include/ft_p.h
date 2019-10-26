/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_p.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 15:25:39 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/21 15:25:41 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_P_H
# define FT_P_H
# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <netdb.h>
# include "../lib/libft/libft.h"
# define FT_P
# define ERROR -1
# define SUCCESS 1
# define STDOUT 1
# define CMDS (char*[]){ "ls", "cd", "get", "put", "pwd", "quit", NULL}

typedef struct		s_env
{
	char			*pwd;
	char			*curr_pwd;
	int				sock;
	int				c_sock;
	int				port;
	char			*cmd;
	char			*res;
	int				error;
	char			buff[4096];
	int				len_read;
}					t_env;


typedef struct 		s_data 
{
	int				size;
	char 			*cmd;
	void			*data;
}					t_data;



void	perform_ls(t_env *e);
#endif
