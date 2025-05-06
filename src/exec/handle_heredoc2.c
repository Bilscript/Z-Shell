/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:06:30 by slebik            #+#    #+#             */
/*   Updated: 2025/05/06 11:36:07 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	here_doc_parent(int *fd)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

static int	handle_doc_exit(int status, int fd)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_exit_status = 130;
		close(fd);
		return (-1);
	}
	else if (WEXITSTATUS(status) == 130)
	{
		g_exit_status = 130;
		close(fd);
		return (-1);
	}
	return (fd);
}

int	handle_here_doc(char *limiter)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	int		tmp;

	tmp = 0;
	if (pipe(fd) == -1)
		error("pipe error");
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		error("fork error");
	}
	if (pid == 0)
		here_doc_child(fd, limiter);
	close(fd[1]);
	tmp = g_exit_status;
	g_exit_status = 102;
	waitpid(pid, &status, 0);
	g_exit_status = tmp;
	return (handle_doc_exit(status, fd[0]));
}
