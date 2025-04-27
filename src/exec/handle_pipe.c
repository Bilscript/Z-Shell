/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:01:23 by slebik            #+#    #+#             */
/*   Updated: 2025/04/26 16:01:23 by slebik           ###   ########.fr       */
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
	int	null_fd;

	if (in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	else if (ft_strcmp(current->cmd, "cat") == 0
		&& !current->args[1] && current->next)
	{
		null_fd = open("/dev/null", O_RDONLY);
		if (null_fd != -1)
		{
			dup2(null_fd, STDIN_FILENO);
			close(null_fd);
		}
	}
	if (current->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	handle_redirections(current);
}

void	exec_command_children(t_command *current, t_envp_list *env_data, int fd)
{
	char	*path;

	if (is_builtin(current->cmd))
	{
		exec_builtin(current, env_data);
		if (fd != 0)
			close(fd);
		exit(EXIT_SUCCESS);
	}
	path = parsing(env_data->head, current->cmd);
	if (!path)
	{
		ft_putstr_fd(current->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		if (fd != 0)
			close(fd);
		exit(127);
	}
	execve(path, current->args, env_data->lenv);
	perror("execve failed");
	free(path);
	exit(1);
}

// Gestion du parent après fork
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
		if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd > 0)
		{
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		redir = redir->next;
	}
}

void	exec_piped_commands(t_command *cmd, t_envp_list *env_data)
{
	int			fd[2];
	int			in_fd;
	pid_t		pid;
	int			status;
	t_command	*current;

	in_fd = 0;
	prepare_heredocs(cmd);
	current = cmd;
	while (current)
	{
		if (current->next && pipe(fd) == -1)
			error("pipe failed");
		pid = fork();
		if (pid == 0)
		{
			prepare_child(current, in_fd, fd);
			exec_command_children(current, env_data, in_fd);
		}
		else if (pid < 0)
			error("fork failed");
		else
			handle_parent(current, &in_fd, fd);
		current = current->next;
	}
	while (wait(&status) > 0);
}
