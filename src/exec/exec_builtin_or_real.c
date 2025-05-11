/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_or_real.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:19:39 by slebik            #+#    #+#             */
/*   Updated: 2025/05/11 15:25:59 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_child_data(t_data *data)
{
	if (data->token)
		free_tokens(data->token);
	if (data->env_data.lenv)
		free_tab(data->env_data.lenv);
}

static void exec_forked_child(t_data *data)
{
	setup_exec_signals();
	if (handle_redirections(data->cmd) == -1)
		exit(1);
	if (is_builtin(data->cmd->cmd))
		exec_builtin(data->cmd, data);
	else
		run_command(data);
	free_child_data(data);
	exit(g_exit_status);
}

static void	handle_signal_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			g_exit_status = 130;
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_exit_status = 131;
			ft_putendl_fd("Quit (core dumped)", 2);
		}
	}
	else
		g_exit_status = WEXITSTATUS(status);
}

static void exec_forked(t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error("fork failed");
	else if (pid == 0)
		exec_forked_child(data);
	g_exit_status = 102;
	waitpid(pid, &status, 0);
	handle_signal_status(status);
}

void exec_builtin_or_real(t_data *data)
{
	t_stdio_backup	backup;

	if (!prepare_heredocs(data))
	{
		g_exit_status = 130;
		return ;
	}
	if (is_builtin(data->cmd->cmd) && !has_heredoc(data->cmd))
	{
		save_stdio(&backup);
		if (handle_redirections(data->cmd) == -1)
		{
			restore_stdio(&backup);
			return ;
		}
		exec_builtin(data->cmd, data);
		restore_stdio(&backup);
	}
	else
		exec_forked(data);
	setup_signals();
}
