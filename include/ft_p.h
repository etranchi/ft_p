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
# include <time.h>
# define FT_P
# define ERROR -1
# define SUCCESS 1
# define STDOUT 1
# define BUFFSIZE 4095
# define CMDS (char*[]){ "ls", "cd", "get", "put", "pwd", "quit", NULL}
# define MIN(a,b) a < b ? a : b

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
	char			buff[BUFFSIZE + 1];
	int				len_read;
}					t_env;

typedef struct		s_data
{
	int				size;
	char			*data;
}					t_data;

void				read_fd_write_fd(t_env *e, int fd_read, int fd_write);
void				go_read_all_buff(t_env *e);
void				perform_ls(t_env *e);
void				send_cmd(t_env *e);
void				error_exit(char *reason);
void				put_msg_on_fd(int fd, char *msg, int free_msg);
t_data				*read_fd(t_env *e, int fd);
void				merge_data(t_data *data, char *buff, int len_read);
void				write_data_on_fd(t_env *e, t_data *data, int fd_write);
void				free_tab(char **tab);
void				free_data(t_data *data);
void				clean_data(char *data);
int					get_status_fd(int fd, int out_fd);

#endif
