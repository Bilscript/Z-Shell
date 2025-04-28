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

int    has_heredoc(t_command *cmd)
{
    t_redir *redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
            return 1;
        redir = redir->next;
    }
    return 0;
}

void	prepare_heredocs(t_command *cmd)
{
	t_command	*current;
	t_redir		*redir;
	int			heredoc_fd;

	current = cmd;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				heredoc_fd = handle_here_doc(redir->filename);
				if (heredoc_fd == -1)
				{
					write(2, "Heredoc interrupted\n", 21);
					exit(130); 
				}
				redir->heredoc_fd = heredoc_fd;
			}
			redir = redir->next;
		}
		current = current->next;
	}
}

void    heredoc_sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);
    exit(130);
}

void	here_doc_child(int *fd, char *limiter)
{
	char	*line;

	signal(SIGINT, heredoc_sigint_handler);
	close(fd[0]);
	while (get_next_line(&line) > 0)
	{
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			close(fd[1]);
			exit(0);
		}
		write_to_pipe(fd[1], line);
	}
	// (EOF / Ctrl+D)
	close(fd[1]);
	exit(0);
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
		error("fork error");
	if (pid == 0)
		here_doc_child(fd, limiter);
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(fd[0]);
		return (-1); // Retourne une err si le child heredoc a ete tuer par SIGINT
	}
	return (fd[0]);
}
