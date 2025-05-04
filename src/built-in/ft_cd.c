/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:26:24 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/04 12:18:32 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_cd_error(char *arg, char *msg, int code)
{
	ft_putstr_fd("cd: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		if (msg)
		{
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(msg, 2);
		}
	}
	else if (msg)
		ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	g_exit_status = code;
}

void	ft_cd(t_command *cmd, t_envp *envp)
{
	char	*path;

	if (cmd->args[1] && cmd->args[2])
		return (print_cd_error(NULL, "too many arguments", EXIT_GENERAL_ERROR));
	if (cmd->args[1])
		path = cmd->args[1];
	else
		path = get_value(envp, "HOME");
	if (!path)
	{
		return (print_cd_error(NULL, "HOME not set", EXIT_GENERAL_ERROR));
	}
	if (chdir(path) != 0)
		return (print_cd_error(path, strerror(errno), EXIT_GENERAL_ERROR));
	g_exit_status = EXIT_SUCCESS;
}
