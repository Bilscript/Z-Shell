/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:13:49 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 17:04:58 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_parent_signals(struct sigaction *old_int,
	struct sigaction *old_quit)
{
	struct sigaction	ignore;

	sigemptyset(&ignore.sa_mask);
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	sigaction(SIGINT, &ignore, NULL);
	sigaction(SIGQUIT, &ignore, NULL);
}

void	restore_parent_signals(struct sigaction *old_int,
	struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

int	pipe_and_fork(t_command *cmd, int *pid, int *fd)
{
	if (cmd->next)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe failed");
			return (-1);
		}
	}
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork failed");
		return (-1);
	}
	return (0);
}

void	exec_child(t_command *cmd, int in_fd, int *fd, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	prepare_child(cmd, in_fd, fd);
	exec_command_children(cmd, fd[1], data);
}

void	exec_piped_loop(t_command *cmd, t_data *data, int *status)
{
	int		in_fd;
	int		fd[2];
	pid_t	pid;

	in_fd = 0;
	while (cmd)
	{
		if (pipe_and_fork(cmd, &pid, fd) == -1)
			return ;
		if (pid == 0)
			exec_child(cmd, in_fd, fd, data);
		handle_parent(cmd, &in_fd, fd);
		cmd = cmd->next;
	}
	while (wait(status) > 0)
		;
}
