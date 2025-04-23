/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/23 15:53:04 by slebik           ###   ########.fr       */
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

void	exec_builtin(t_command *cmd, t_envp *envp)
{
	if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(cmd, envp);
	//else if (!ft_strcmp(cmd->cmd, "unset"))
		//ft_unset(cmd);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, envp);
	//else if (!ft_strcmp(cmd->cmd, "exit"))
		//ft_exit(cmd);
}

static void	exec_builtin_or_real(t_command *cmd, t_envp *envp)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		ft_export(cmd, envp);
	//else if (ft_strcmp(cmd->cmd, "unset") == 0)
	//	ft_unset(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_env(cmd, envp);
	//	else if (ft_strcmp(cmd->cmd, "exit") == 0)
	//	ft_exit(cmd);
	else
	{
		run_command(cmd,envp);
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

void	exec(t_command *cmd_line, t_envp *envp, t_token *token)
{
	if (has_pipe(token)) // si ya au moins un pipe
		exec_piped_commands(cmd_line, envp);
	else // si ya qu'une cmd
	{
		while (cmd_line)
		{
			if (cmd_line->cmd)
				exec_builtin_or_real(cmd_line, envp);
			cmd_line = cmd_line->next;
		}
	}
}
