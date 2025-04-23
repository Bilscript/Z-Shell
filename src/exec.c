/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:38:30 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/23 12:45:53 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_builtin(t_command *cmd, t_envp *envp)
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
		printf("(%s): Command not found\n", cmd->cmd);
		// free tard plus
	}
}

void	exec(t_command *cmd_line, t_envp *envp)
{
	while (cmd_line)
	{
		if (cmd_line->cmd)
			exec_builtin(cmd_line, envp);
		cmd_line = cmd_line->next;
	}
}
