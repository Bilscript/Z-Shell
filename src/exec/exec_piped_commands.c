/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_piped_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:13:49 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 13:00:59 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
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
				free_all(NULL, data);
				exit(1);
			}
			close(in_fd);
		}
		if (handle_redirections(curr) == -1)
		{
			free_all(NULL, data);
			exit(1);
		}
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
*/
#include <signal.h>

void	exec_piped_commands(t_data *data)
{
	t_command			*current = data->cmd;
	int					in_fd = 0;
	int					fd[2];
	pid_t				pid;
	pid_t				last_pid = -1;
	struct sigaction	old_int, old_quit, ignore;
	int					status;

	// Ignorer SIGINT et SIGQUIT dans le parent
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	sigaction(SIGINT, NULL, &old_int);
	sigaction(SIGQUIT, NULL, &old_quit);
	sigaction(SIGINT, &ignore, NULL);
	sigaction(SIGQUIT, &ignore, NULL);
	(void)last_pid;
	while (current)
	{
		if (current->next && pipe(fd) == -1)
		{
			perror("pipe failed");
			return ;
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			return ;
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			prepare_child(current, in_fd, fd);
			exec_command_children(current, fd[1], data);
		}
		else
		{
			last_pid = pid;
			handle_parent(current, &in_fd, fd);
			current = current->next;
		}
	}
	
	// Attendre tous les enfants
	while (wait(&status) > 0)
		;

	// Restaurer les signaux
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);

	// Mise à jour du statut de sortie
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

