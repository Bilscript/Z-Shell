/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:39:53 by slebik            #+#    #+#             */
/*   Updated: 2025/04/26 17:39:53 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_output_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_heredoc_redir(t_redir *redir)
{
	if (redir->heredoc_fd >= 0)
	{
		if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 heredoc");
			close(redir->heredoc_fd);
			return (-1);
		}
		close(redir->heredoc_fd);
	}
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN && handle_input_redir(redir) == -1)
			return (-1);
		else if (redir->type == TOKEN_REDIR_OUT
			&& handle_output_redir(redir) == -1)
			return (-1);
		else if (redir->type == TOKEN_APPEND
			&& handle_append_redir(redir) == -1)
			return (-1);
		else if (redir->type == TOKEN_HEREDOC
			&& handle_heredoc_redir(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
