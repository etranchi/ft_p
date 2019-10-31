/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform_local_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etranchi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/31 14:42:36 by etranchi          #+#    #+#             */
/*   Updated: 2019/10/31 14:42:37 by etranchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_p.h"

void					perform_lls(t_env *e)
{
	pid_t				pid;
	char				**cmd;

	pid = fork();
	if (pid == -1)
		error_exit("Fork.\n");
	if (pid == 0)
	{
		put_msg_on_fd(STDOUT, "SUCCESS | lls", 0);
		cmd = ft_strsplit(e->cmd, ' ');
		if (cmd[1] && cmd[1][0] != '-')
			cmd[1] = ".";
		else
			cmd[1] = check_ls_cmd(cmd[1]);
		if (cmd[2])
			cmd[2] = ".";
		execv("/bin/ls", cmd);
		free_tab(cmd);
	}
	wait4(pid, 0, 0, NULL);
}

void					perform_lcd(t_env *e)
{
	char				**tab_cmd;
	char				*tmp_pwd;

	tab_cmd = ft_strsplit(e->cmd, ' ');
	if (chdir(tab_cmd[1] ? tab_cmd[1] : ".") == -1 && free_tab(tab_cmd))
		return (put_msg_on_fd(STDOUT, "ERROR | lcd", 0));
	tmp_pwd = getwd(NULL);
	if (ft_strlen(tmp_pwd) < ft_strlen(e->pwd) && chdir(e->pwd) != -1)
		put_msg_on_fd(STDOUT,
			"ERROR : You don't have the rights to do this.", 0);
	else
	{
		if (e->curr_pwd != NULL)
			free(e->curr_pwd);
		if (!tab_cmd[1])
			e->curr_pwd = ft_strdup(e->pwd);
		else
			e->curr_pwd = ft_strdup(tmp_pwd);
		put_msg_on_fd(STDOUT,
			ft_strjoin_free("SUCCESS | lcd | ", e->curr_pwd, 0, 0), 1);
		chdir(e->curr_pwd);
	}
	free(tmp_pwd);
	free_tab(tab_cmd);
}

void					perform_lpwd(t_env *e)
{
	put_msg_on_fd(STDOUT, "SUCCESS | lpwd", 0);
	put_msg_on_fd(STDOUT, e->curr_pwd ? e->curr_pwd : e->pwd, 0);
}

void					perform_quit(void)
{
	put_msg_on_fd(STDOUT, "SUCCESS | quit.", 0);
	exit(1);
}
