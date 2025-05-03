/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:29:45 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/03 14:59:11 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_key(t_envp **head, t_envp *node_to_remove)
{
	t_envp	*current;
	t_envp	*prev;

	prev = NULL;
	current = *head;
	if (current == node_to_remove)
	{
		*head = current->next;
		free(current->key);
		free(current->value);
		free(current);
		return ;
	}
	while (current != NULL && current != node_to_remove)
	{
		prev = current;
		current = current->next;
	}
	if (current == node_to_remove)
	{
		prev->next = current->next;
		free(current->key);
		free(current->value);
		free(current);
	}
}

void	ft_unset(t_command *cmd, t_envp *envp)
{
	t_envp	*temp;

	if (!cmd->args[1])
	{
		g_exit_status = EXIT_SUCCESS;
		return ;
	}
	temp = envp;
	while (temp)
	{
		if (ft_strcmp(temp->key, cmd->args[1]) == 0)
		{
			remove_key(&envp, temp);
			g_exit_status = EXIT_SUCCESS;
			return ;
		}
		temp = temp->next;
	}
	g_exit_status = EXIT_SUCCESS;
}
