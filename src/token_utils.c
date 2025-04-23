/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamani <bhamani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:22:04 by bhamani           #+#    #+#             */
/*   Updated: 2025/04/14 18:22:04 by bhamani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '"' || c == '\'' || c == '$');
}

void print_tokens(t_token *list)
{
	const char *g_token_names[] =
	{
		"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND", "DQUOTE", "SQUOTE", "TOKEN_VAR", "TOKEN_EOF"
	};

	const char *g_quote_status[] =
	{
		"QUOTE_NONE", "QUOTE_SINGLE", "QUOTE_DOUBLE"
	};
	
	while (list) 
	{
		printf("Type: %s, Value: [%s] , Quote status: %s \n", g_token_names[list->type], list->value, g_quote_status[list->quote_status]);
		list = list->next;
	}
}
