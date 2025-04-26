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

void prepare_heredocs(t_command *cmd)
{
    t_command *current;

	current = cmd;
    while (current)
    {
        t_redir *redir = current->redirs;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC)
            {
                int heredoc_fd = handle_here_doc(redir->filename);
                redir->heredoc_fd = heredoc_fd;
            }
            redir = redir->next;
        }
        current = current->next;
    }
}

void here_doc_child(int *fd, char *limiter)
{
    char *line;

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
    close(fd[1]);
    exit(0);
}

void here_doc_parent(int *fd)
{
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
}

int handle_here_doc(char *limiter)
{
    int     fd[2];
    pid_t   pid;

    if (pipe(fd) == -1)
        error("pipe error");
    pid = fork();
    if (pid == -1)
        error("fork error");
    if (pid == 0)
        here_doc_child(fd, limiter);
    close(fd[1]);
    waitpid(pid, NULL, 0);
    return (fd[0]);
}
