/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/20 20:41:06 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_builtin(t_command *cmd, char **envp)
{
	if (ft_strcmp(cmd->cmd, "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->cmd, "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		ft_pwd();
	//else if (ft_strcmp(cmd->cmd, "export") == 0)
	//	ft_export(cmd);
	//else if (ft_strcmp(cmd->cmd, "unset") == 0)
	//	ft_unset(cmd);
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		ft_env(cmd, envp);
	//	else if (ft_strcmp(cmd->cmd, "exit") == 0)
	//	ft_exit(cmd);
	else
	{
		printf("(%s): Command not found\n", cmd->cmd);
		// free tard plus
		getenv("");
	}
}

void	exec(t_command *cmd_line, char **envp)
{
	while (cmd_line)
	{
		if (cmd_line->cmd)
			exec_builtin(cmd_line, envp);
		cmd_line = cmd_line->next;
	}
}
