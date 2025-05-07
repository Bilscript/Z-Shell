/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:03:52 by slebik            #+#    #+#             */
/*   Updated: 2025/05/05 14:03:52 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	prepare_child(t_command *current, int in_fd, int *fd)
{
	setup_exec_signals();
	if (in_fd != 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
		close(in_fd);
	}
	if (current->next)
	{
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(1);
		}
		close(fd[1]);
	}
	if (handle_redirections(current) == -1)
		exit(1);
}

void	exec_command_children(t_command *current, t_envp_list *env_data, int fd)
{
	char	*path;

	(void)fd;
	if (is_builtin(current->cmd))
	{
		exec_builtin(current, env_data, NULL);
		exit(g_exit_status);
	}
	path = parsing(env_data->head, current->cmd);
	if (!path)
	{
		ft_putstr_fd(current->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free_command(current);
		free_envp_list(env_data);
		exit(127);
	}
	execve(path, current->args, env_data->lenv);
	perror("execve failed");
	free(path);
	exit(1);
}

void	handle_parent(t_command *current, int *in_fd, int *fd)
{
	t_redir	*redir;

	if (*in_fd != 0)
		close(*in_fd);
	if (current->next)
	{
		close(fd[1]);
		*in_fd = fd[0];
	}
	redir = current->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd >= 0)
		{
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}
