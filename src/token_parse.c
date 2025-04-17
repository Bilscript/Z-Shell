/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:57:58 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/17 17:06:44 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *parse_tokens(t_token *token)
{
	t_command *cmds = NULL;
	t_command *current = NULL;

	while (token)
	{
		if (!current)
		{
			current = calloc(1, sizeof(t_command));
			if (!cmds)
				cmds = current;
		}

		if (token->type == TOKEN_WORD)
			add_arg(current, token->value);
		else if (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
				|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
		{
			token = token->next;
			if (!token || token->type != TOKEN_WORD)
			{
				printf("Erreur de redirection : fichier manquant\n");
				return NULL;
			}
			add_redir(current, token->prev->type, token->value);
		}
		else if (token->type == TOKEN_PIPE)
		{
			current->next = calloc(1, sizeof(t_command));
			current = current->next;
		}
		token = token->next;
	}
	return cmds;
}

t_command *parse_tokens(t_token *token)
{
	
}