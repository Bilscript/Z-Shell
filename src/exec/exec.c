/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/10 14:26:14 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec(t_data *data)
{
	t_command	*current;
	t_command	*next;

	if (has_pipe(data->token))
		exec_piped_commands(data);
	else
	{
		current = data->cmd;
		while (current)
		{
			next = current->next;
			if (current->cmd)
				exec_builtin_or_real(data);
			current = next;
		}
	}
}


#include <signal.h>

static int	fork_and_exec(char *path, t_data *data)
{
	pid_t	pid;
	int		status;
	struct sigaction	sa_int_old;
	struct sigaction	sa_quit_old;
	struct sigaction	sa_ignore;

	// Préparer une action ignorée
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;

	// Sauvegarder les signaux actuels et ignorer SIGINT/SIGQUIT dans le parent
	sigaction(SIGINT, NULL, &sa_int_old);
	sigaction(SIGQUIT, NULL, &sa_quit_old);
	sigaction(SIGINT, &sa_ignore, NULL);
	sigaction(SIGQUIT, &sa_ignore, NULL);

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		g_exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		// Restaure les signaux défauts dans le fork
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(path, data->cmd->args, data->env_data.lenv);
		perror("execve failed");
		free(path);
		exit(126);
	}

	waitpid(pid, &status, 0);

	// Restauration des signaux par défaut dans le parent
	sigaction(SIGINT, &sa_int_old, NULL);
	sigaction(SIGQUIT, &sa_quit_old, NULL);

	free(path);

	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);  // Ex. 130 pour Ctrl+C

	return (0);
}


void run_command(t_data *data)
{
	char	*path;

	if (!data->cmd || !data->cmd->cmd)
		return ;
	path = parsing(data->env_data.head, data->cmd->cmd);
	if (!check_cmd_path(&path, data->cmd))
	{
		free(path);
		return ;
	}
	if (fork_and_exec(path, data))
	{
		free_all(NULL, data);
		return ;
	}
}
