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

void	print_tokens(t_token *list)
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

void	print_commands(t_command *cmd_list)
{
	const char *redir_types[] = {
		"REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC"
	};

	while (cmd_list)
	{
		printf("=== Command ===\n");
		printf("Command: %s\n", cmd_list->cmd ? cmd_list->cmd : "(null)");
		printf("Arguments:\n");
		if (cmd_list->args)
		{
			for (int i = 0; cmd_list->args[i]; i++)
				printf("  - [%s]\n", cmd_list->args[i]);
		}
		else
			printf("  (none)\n");
		printf("Redirections:\n");
		t_redir *redir = cmd_list->redirs;
		if (!redir)
			printf("  - (Aucune redirection)\n");
		while (redir)
		{
			printf("  - Type: %s, File: [%s]\n", redir_types[redir->type - TOKEN_REDIR_IN], redir->filename);
			redir = redir->next;
		}
		printf("\n");
		cmd_list = cmd_list->next;
	}
}
