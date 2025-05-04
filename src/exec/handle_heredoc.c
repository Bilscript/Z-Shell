/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:59:00 by slebik            #+#    #+#             */
/*   Updated: 2025/04/26 16:59:00 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc(t_redir *redir)
{
	int	fd;

	fd = handle_here_doc(redir->filename);
	if (fd == -1)
		return (0);
	redir->heredoc_fd = fd;
	return (1);
}

int	has_heredoc(t_command *cmd)
{
	t_redir	*redir;

	if (!cmd || !cmd->redirs)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(t_command *cmd)
{
	t_command	*current;
	t_redir		*redir;

	current = cmd;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				if (!process_heredoc(redir))
					return (0);
				if (g_exit_status == 130)
					return (0);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (1);
}

void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	g_exit_status = 130;
	write(1, "\n", 1);
	close(0); // Fermer stdin pour interrompre get_next_line ou readline
}

void	here_doc_child(int *fd, char *limiter)
{
	char	*line;

	setup_heredoc_signals();
	close(fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			// Soit EOF, soit interruption
			close(fd[1]);
			exit(g_exit_status);
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			close(fd[1]);
			exit(0);
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}

void	here_doc_parent(int *fd)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

int	handle_here_doc(char *limiter)
{
	int		fd[2];
	pid_t	pid;
	int		status;

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
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_exit_status = 130;
		close(fd[0]);
		return (-1);
	}
	else if (WEXITSTATUS(status) == 130)
	{
		g_exit_status = 130;
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}
// mtn ca gere un heredoc pour une redirection spécifique et ca retourne le descripteur de fichier ou -1 en cas d'erreur ou d'interruption
