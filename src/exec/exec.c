/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/06 10:09:10 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_command *cmd_line, t_envp_list *env_data, t_token *token)
{
	if (has_pipe(token))
		exec_piped_commands(cmd_line, env_data);
	else
	{
		while (cmd_line)
		{
			if (cmd_line->cmd)
				exec_builtin_or_real(cmd_line, env_data);
			cmd_line = cmd_line->next;
		}
	}
}

static int	fork_and_exec(char *path, t_command *cmd, t_envp_list *env_data)
{
	pid_t	pid;
	int		status;

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
		execve(path, cmd->args, env_data->lenv);
		perror("execve failed");
		free(path);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	return (0);
}

void	run_command(t_command *cmd, t_envp_list *env_data)
{
	char	*path;

	if (!cmd || !cmd->cmd)
		return ;
	path = parsing(env_data->head, cmd->cmd);
	if (!check_cmd_path(&path, cmd))
		return ;
	if (fork_and_exec(path, cmd, env_data))
		return ;
	free(path);
}
