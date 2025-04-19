/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:26 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/17 17:56:49 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_args(t_command *line_cmd, const char *args)
{
	int		i;
	char	**new_args;

	
	i = 0;
	if (!line_cmd->cmd)
	{
		line_cmd->cmd = new_args[0];
	}
	
	
}

t_command *new_command(void)
{
	t_command *cmd = malloc(sizeof(t_command));
	if (!cmd)
		return NULL;
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return cmd;
}

t_redir *new_redir(int type, const char *filename)
{
	t_redir *redir = malloc(sizeof(t_redir));
	if (!redir)
		return NULL;
	redir->type = type;
	redir->filename = strdup(filename);
	redir->next = NULL;
	return redir;
}

void add_redir(t_command *cmd, t_redir *redir)
{
	t_redir *tmp;

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

void add_arg(t_command *cmd, const char *word)
{
	int i;
	int j;
	char **new_args;

	i = 0;
	if (cmd->args)
		while (cmd->args[i])
			i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return;
	j = 0;
	while (j < i)
	{
		new_args[j] = cmd->args[j];
		j++;
	}
	new_args[i] = strdup(word);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	if (cmd->cmd == NULL)
		cmd->cmd = new_args[0];
}
