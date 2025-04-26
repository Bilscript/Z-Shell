/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/25 17:08:44 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

void	exec_builtin(t_command *cmd, t_envp_list *env_data)
{
	if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(cmd, env_data->head);
	//else if (!ft_strcmp(cmd->cmd, "unset"))
		//ft_unset(cmd);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, env_data->head);
	//else if (!ft_strcmp(cmd->cmd, "exit"))
		//ft_exit(cmd);
}

static void	exec_builtin_or_real(t_command *cmd, t_envp_list *env_data)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		ft_export(cmd, env_data->head);
	//else if (ft_strcmp(cmd->cmd, "unset") == 0)
	//	ft_unset(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_env(cmd, env_data->head);
	//	else if (ft_strcmp(cmd->cmd, "exit") == 0)
	//	ft_exit(cmd);
	else
	{
		run_command(cmd, env_data);
		// free tard plus
		//getenv("");
	}
}
int	has_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

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
