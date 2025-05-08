/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/08 11:50:58 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_command *cmd_line, t_envp_list *env_data, t_token *token)
{
	if (has_pipe(token))
		exec_piped_commands(cmd_line, env_data, token);
	else
	{
		while (cmd_line)
		{
			if (cmd_line->cmd)
				exec_builtin_or_real(cmd_line, env_data, token);
			cmd_line = cmd_line->next;
		}
	}
}

static int	fork_and_exec(char *path, t_command *cmd, t_envp_list *env_data,
				t_token *token)
{
	pid_t	pid;
	int		status;

	(void)token;
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
	printf("debug\n\n\n");
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	free_tokens(token);
	return (0);
}

void	run_command(t_command *cmd, t_envp_list *env_data, t_token *token)
{
	char	*path;

	if (!cmd || !cmd->cmd)
		return ;
	path = parsing(env_data->head, cmd->cmd);
	if (!check_cmd_path(&path, cmd))
	{
		free_tokens(token);
		return ;
	}
	if (fork_and_exec(path, cmd, env_data, token))
	{
		return ;
	}
	free(path);
}
