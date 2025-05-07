/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_directory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 12:12:16 by slebik            #+#    #+#             */
/*   Updated: 2025/05/07 20:27:38 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	not_directory(char **path, t_command *cmd)
{
	struct stat	path_stat;

	if (stat(*path, &path_stat) == -1)
	{
		perror(*path);
		free(*path);
		*path = NULL;
		g_exit_status = 127;
		return (0);
	}
	if (!S_ISREG(path_stat.st_mode))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(*path);
		*path = NULL;
		g_exit_status = 126;
		return (0);
	}
	return (1);
}

int	check_cmd_path(char **path, t_command *cmd)
{
	if (!*path && ft_strchr(cmd->args[0], '/'))
	{
		perror(cmd->args[0]);
		g_exit_status = 126;
		return (0);
	}
	if (!*path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		g_exit_status = 127;
		return (0);
	}
	if (access(*path, X_OK) == -1)
	{
		free_command(cmd);
		perror(*path);
		free(*path);
		*path = NULL;
		g_exit_status = 126;
		return (0);
	}
	return (not_directory(path, cmd));
}
