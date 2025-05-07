/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:04:18 by slebik            #+#    #+#             */
/*   Updated: 2025/05/08 00:38:30 by bhamani          ###   ########.fr       */
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

void	exec_builtin(t_command *cmd, t_envp_list *env_data, t_token *token)
{
	prepare_heredocs(cmd);
	if (!ft_strcmp(cmd->cmd, "echo"))
		ft_echo(cmd);
	else if (!ft_strcmp(cmd->cmd, "cd"))
		ft_cd(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(cmd->cmd, "export"))
		ft_export(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		ft_unset(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "env"))
		ft_env(cmd, env_data->head);
	else if (!ft_strcmp(cmd->cmd, "exit"))
		ft_exit(cmd, env_data, token);
}
