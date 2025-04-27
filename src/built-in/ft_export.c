/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:12:19 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 13:11:44 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_key(t_envp *envp, char *key, char *value)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
		{
			free(envp->value);
			envp->value = ft_strdup(value + len_until_char(value, '=') + 1);
			return (1);
		}
		envp = envp->next;
	}
	return (0);
}

void	ft_export(t_command *cmd, t_envp *envp)
{
	t_envp	*temp;
	int		len;

	temp = envp;
	if (!cmd->args[1])
	{
		while (temp)
		{
			if (temp->export == true)
				printf("export %s=%s\n", temp->key, temp->value);
			temp = temp->next;
		}
		return ;
	}
	len = len_until_char(cmd->args[1], '=');
	if (set_key(envp, ft_strndup(cmd->args[1], len), cmd->args[1]))
		return ;
	add_envp_back(&envp, new_envp(ft_strndup(cmd->args[1],
				len), cmd->args[1] + len + 1, true));
}
/*
void	ft_unset(t_command *cmd, t_envp *envp)
{
	if (!cmd->args[1])
		return ;
	
}
*/