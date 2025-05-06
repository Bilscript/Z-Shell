/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:04:04 by slebik            #+#    #+#             */
/*   Updated: 2025/05/05 14:04:04 by slebik           ###   ########.fr       */
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
	close(0);
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
