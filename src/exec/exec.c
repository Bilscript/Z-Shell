/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 13:43:48 by bhamani          ###   ########.fr       */
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


static int	fork_and_exec(char *path, t_data *data)
{
	pid_t				pid;
	int					status;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;
	struct sigaction	sa_ignore;

	// Préparer struct pour ignorer SIGINT/SIGQUIT dans le parent
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;

	// Sauvegarder et remplacer temporairement les signaux
	sigaction(SIGINT, NULL, &sa_old_int);
	sigaction(SIGQUIT, NULL, &sa_old_quit);
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
		// Restaure comportements défaut dans le fork (CTRL+C, CTRL+\)
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(path, data->cmd->args, data->env_data.lenv);
		perror("execve failed");
		free(path);
		exit(126);
	}

	// Parent attend le fils
	waitpid(pid, &status, 0);

	// Restaure signaux initiaux
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);

	free(path);

	// Met à jour le code de retour
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);

	return (0);
}

void run_command(t_data *data)
{
	char *path;

	if (!data->cmd || !data->cmd->cmd)
		exit(0); // Rien à exécuter, on quitte proprement

	path = parsing(data->env_data.head, data->cmd->cmd);
	if (!check_cmd_path(&path, data->cmd))
	{
		// On affiche l'erreur et on quitte sans double free
		ft_putstr_fd(data->cmd->cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		free(path); // important : path est alloué
		exit(127);
	}

	// Fork + execve
	if (fork_and_exec(path, data))
		exit(g_exit_status); // fork_and_exec a déjà free(path)

	exit(g_exit_status);
}


