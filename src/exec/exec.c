/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 17:20:49 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_signals_ignore(struct sigaction *old_int,
	struct sigaction *old_quit)
{
	struct sigaction	ignore;

	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	sigaction(SIGINT, &ignore, NULL);
	sigaction(SIGQUIT, &ignore, NULL);
}

static void	restore_signals(struct sigaction *old_int,
	struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

static void	exec_child_process(char *path, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, data->cmd->args, data->env_data.lenv);
	perror("execve failed");
	free(path);
	exit(126);
}

static int	fork_and_exec(char *path, t_data *data)
{
	pid_t				pid;
	int					status;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	setup_signals_ignore(&old_int, &old_quit);
	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		free(path);
		g_exit_status = 1;
		return (1);
	}
	if (pid == 0)
		exec_child_process(path, data);
	waitpid(pid, &status, 0);
	restore_signals(&old_int, &old_quit);
	free(path);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	return (0);
}

void	run_command(t_data *data)
{
	char	*path;

	if (!data->cmd || !data->cmd->cmd)
		exit(0);
	path = parsing(data->env_data.head, data->cmd->cmd);
	if (!check_cmd_path(&path, data->cmd))
	{
		free(path);
		exit(127);
	}
	if (fork_and_exec(path, data))
		exit(g_exit_status);
	exit(g_exit_status);
}
