/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:28:46 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 11:28:56 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_command *cmd, t_envp *envp)
{
	if (cmd->args[1])
	{
		printf("cd: too many arguments\n");
		return ;
	}
	while (envp)
	{
		printf("%s=%s\n", envp->key, envp->value);
		envp = envp->next;
	}
}
