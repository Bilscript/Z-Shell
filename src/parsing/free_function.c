/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slebik <slebik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:43:28 by bhamani           #+#    #+#             */
/*   Updated: 2025/05/11 16:09:07 by slebik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->value);
		free(list);
		list = tmp;
	}
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp_redir;

	while (redir)
	{
		tmp_redir = redir;
		if (redir->filename)
		{
			free(redir->filename);
			redir->filename = NULL;
		}
		redir = redir->next;
		free(tmp_redir);
		tmp_redir = NULL;
	}
}

void	free_command(t_command *cmd)
{
	t_command	*next;

	if (cmd)
	{
		while (cmd)
		{
			next = cmd->next;
			if (cmd->args)
				free_args(cmd->args);
			if (cmd->redirs)
				free_redirs(cmd->redirs);
			free(cmd);
			cmd = next;
		}
	}
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}
