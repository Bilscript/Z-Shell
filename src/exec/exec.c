/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/08 19:27:00 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_data *data)
{
	if (has_pipe(data->token))
		exec_piped_commands(data);
	else
	{
		while (data->cmd)
		{
			if (data->cmd->cmd)
			{
				exec_builtin_or_real(data);
			}
			data->cmd = data->cmd->next;
		}
	}
}

static int	fork_and_exec(char *path, t_data *data)
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
		execve(path, data->cmd->args, data->env_data.lenv);
		perror("execve failed");
		free(path);
		exit(126);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	//free_tokens(data->token);  nrml c gerer apres 
	return (0);
}

void	run_command(t_data *data)
{
	char	*path;

	if (!data->cmd || !data->cmd->cmd)
		return ;
	path = parsing(data->env_data.head, data->cmd->cmd);
	if (!check_cmd_path(&path, data->cmd))
	{
		free_tokens(data->token);
		return ;
	}
	if (fork_and_exec(path, data))
	{
		return ;
	}
	free(path);
}
