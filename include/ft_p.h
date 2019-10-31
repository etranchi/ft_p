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
# include <dirent.h>
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
# define L "ls"
# define C "cd"
# define P "pwd"
# define LP "lpwd"
# define LL "lls"
# define LC "lcd"
# define Q "quit"
# define M "mkdir"
# define R "rmdir"
# define U "unlink"
# define N NULL
# define CMDS (char*[]){ L, C, "get", "put", P, Q, LL, LC, LP, M, R, U, N}
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

/*
**	server
*/
void				send_cmd(t_env *e);

/*
**	perform_server_cmd
*/
void				perform_get(t_env *e);
void				perform_put(t_env *e);
void				perform_pwd(t_env *e);
void				perform_cd(t_env *e);
void				perform_ls(t_env *e);

/*
**	perform_dir_cmd
*/
void				perform_mkdir(t_env *e);
void				perform_rmdir(t_env *e);
void				perform_unlink(t_env *e);

/*
**	perform_local_cmd
*/
void				perform_lls(t_env *e);
void				perform_lcd(t_env *e);
void				perform_lpwd(t_env *e);
void				perform_quit(void);

/*
**	send_dir_cmd
*/
void				send_rmdir(t_env *e);
void				send_mkdir(t_env *e);

/*
**	perform_cmd
*/
void				put_file(t_env *e);
void				get_cd(t_env *e);
void				get_file(t_env *e);
void				get_pwd(t_env *e);
void				get_ls(t_env *e);

/*
**	utils_pwd
*/
void				set_pwd(t_env *e);
char				*check_ls_cmd(char *cmd);

/*
**	utils_fd
*/
void				put_msg_on_fd(int fd, char *msg, int free_msg);
t_data				*read_fd(t_env *e, int fd);
void				merge_data(t_data *data, char *buff, int len_read);
void				read_fd_write_fd(t_env *e, int fd_read, int fd_write);
void				write_data_on_fd(t_env *e, t_data *data, int fd_write);

/*
**	utils
*/
void				error_exit(char *reason);
int					get_status_fd(int fd, int out_fd);
char				*ft_strjoin_free(char const *s1,
	char const *s2, int f1, int f2);
void				error(t_env *e, char *reason, char **tab);

/*
**	free_all
*/
int					free_tab(char **tab);
void				free_data(t_data *data);

#endif
