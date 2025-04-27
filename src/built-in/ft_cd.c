/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:26:24 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 11:26:52 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_command *cmd_line)
{
	char	*path;

	if (cmd_line->args[2])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (!cmd_line->args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME is not set\n", 21);
			return ;
		}
	}
	else
		path = cmd_line->args[1];
	if (chdir(path) != 0)
	{
		write(2, "cd: ", 4);
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
		perror("");
	}
}
