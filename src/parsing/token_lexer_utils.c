/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lexer_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:26 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/27 11:48:33 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

t_redir	*new_redir(int type, const char *filename)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = strdup(filename);
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_command *cmd, t_redir *redir)
{
	t_redir	*tmp;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
}

static char	**copy_args(char **args, int count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = NULL;
	new_args[i + 1] = NULL;
	return (new_args);
}

void	add_arg(t_command *cmd, const char *word)
{
	char	**new_args;
	int		i;

	i = 0;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	new_args = copy_args(cmd->args, i);
	if (!new_args)
		return ;
	new_args[i] = strdup(word);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	if (!cmd->cmd)
		cmd->cmd = cmd->args[0];
}
