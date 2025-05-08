/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:13:49 by slebik            #+#    #+#             */
/*   Updated: 2025/05/08 17:39:33 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child(t_command *curr, int *fd, int in_fd, t_data *data)
{
	setup_exec_signals();
	if (curr->next)
		prepare_child(curr, in_fd, fd);
	else
	{
		if (in_fd != 0)
		{
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 stdin");
				exit(1);
			}
			close(in_fd);
		}
		if (handle_redirections(curr) == -1)
			exit(1);
	}
	exec_command_children(curr, in_fd, data);
}

static void	wait_all_children(void)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				g_exit_status = 0;
			}
			else if (WTERMSIG(status) == SIGQUIT)
				g_exit_status = 131;
		}
		else
		{
			g_exit_status = WEXITSTATUS(status);
		}
	}
}

void	exec_piped_commands(t_data *data)
{
	int			fd[2];
	int			in_fd;
	pid_t		pid;
	t_command	*cur;

	in_fd = 0;
	cur = data->cmd;
	if (!prepare_heredocs(data))
		return ;
	while (cur)
	{
		if (cur->next && pipe(fd) == -1)
			error("pipe failed");
		pid = fork();
		if (pid == 0)
			handle_child(cur,fd, in_fd, data);
		else if (pid < 0)
			error("fork failed");
		handle_parent(cur, &in_fd, fd);
		cur = cur->next;
	}
	wait_all_children();
	setup_signals();
}
