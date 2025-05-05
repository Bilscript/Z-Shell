/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin_or_real.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:19:39 by slebik            #+#    #+#             */
/*   Updated: 2025/05/05 16:41:19 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_forked_child(t_command *cmd, t_envp_list *env_data)
{
	setup_exec_signals();
	if (handle_redirections(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->cmd))
		exec_builtin(cmd, env_data);
	else
		run_command(cmd, env_data);
	exit(g_exit_status);
}

static void	handle_signal_status(int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			g_exit_status = 130;
			write(1, "\n", 1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			g_exit_status = 131;
			ft_putendl_fd("Quit (core dumped)", 2);
		}
	}
	else
		g_exit_status = WEXITSTATUS(status);
}

static void	exec_forked(t_command *cmd, t_envp_list *env_data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		error("fork failed");
	else if (pid == 0)
		exec_forked_child(cmd, env_data);
	waitpid(pid, &status, 0);
	handle_signal_status(status);
}

void	exec_builtin_or_real(t_command *cmd, t_envp_list *env_data)
{
	t_stdio_backup	backup;

	if (!prepare_heredocs(cmd) || g_exit_status == 130)
		return ;
	if (is_builtin(cmd->cmd) && !has_heredoc(cmd))
	{
		save_stdio(&backup);
		if (handle_redirections(cmd) == -1)
		{
			restore_stdio(&backup);
			return ;
		}
		exec_builtin(cmd, env_data);
		restore_stdio(&backup);
	}
	else
		exec_forked(cmd, env_data);
	setup_signals();
}
